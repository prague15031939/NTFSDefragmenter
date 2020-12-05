#include "pch.h"
#include "Core.h"
#include "winioctl.h"
#include <sstream>
#include <string>
#include <queue>

CRITICAL_SECTION criticalSection;
std::vector<DefragmentationLogItem*> DefragmentationLogs;
std::queue<DefragmentationLogItem*> testLog;

VOLUME_BITMAP_BUFFER* readVolumeBitmap(LPCWSTR drive);
RETRIEVAL_POINTERS_BUFFER* readFileBitmap(std::wstring fileName);
int Move(LPCWSTR lpSrcName, LPCWSTR drive);
void createLog(const wchar_t* res, ATL::CString fullName);
void createTestLog();

const wchar_t* directories[10] = {
            L"D:\\test1\\1.txt",
            L"D:\\test1\\2.txt",
            L"D:\\test1\\3.txt",
            L"D:\\test2\\1.txt",
            L"D:\\test2\\2.txt",
            L"D:\\test2\\3.txt",
            L"D:\\test3\\1.txt",
            L"D:\\test3\\2.txt",
            L"D:\\test3\\3.txt",
            L"D:\\test3\\4.txt",
};

// обрабатываем файлы на диске
int __cdecl WorkIn(CString directory, CString dr, bool first)
{
    InitializeCriticalSectionAndSpinCount(&criticalSection, 5000);

    int res = 1;
    // если вызов для корневой директории, то добавить #:\ чтобы было C:\ 
    if (first) directory += ":\\";

    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    hf = FindFirstFile((directory + L"*"), &FindFileData);
    if (hf != INVALID_HANDLE_VALUE)
    {
        res = -1;
        do
        {
            CString tmp = FindFileData.cFileName;
            // если нам попались папки . или .., то пропускаем
            if (tmp != "." && tmp != "..")
            {
                CString full_file_name = directory + FindFileData.cFileName;
                // если попалась папка, то вызываем рекурсивно функцию еще раз для папки
                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    WorkIn(full_file_name + "\\", dr);
                }
                else
                {
                    // получаем расположение файла
                    RETRIEVAL_POINTERS_BUFFER* fileBitmap = readFileBitmap(full_file_name.GetString());
                    if (fileBitmap != NULL)
                    {
                        // если файл не разбил на части, то выводим +
                        if (fileBitmap->ExtentCount == 1)
                        {
                            createLog(L"+", full_file_name);
                        }
                        else 
                        {
                            // если разбит, то выполняем его дефрагментацию
                            // если дефрагментация удалась, то выводим = иначе -
                            res = Move(full_file_name.GetString(), dr.GetString());
                            createLog((!res) ? L"=" : L"-", full_file_name);
                        }
                    }
                    free(fileBitmap);
                }
            }

        } while (FindNextFile(hf, &FindFileData) != 0);

        FindClose(hf);
    }

    DeleteCriticalSection(&criticalSection);

    return res;
}

// функция чтения битовой карты диска с указанием свободных/занятых кластеров
VOLUME_BITMAP_BUFFER* readVolumeBitmap(LPCWSTR drive)
{
    HANDLE hDrive = 0;
    DWORD Bytes;
    int ret;
    _int64 nOutSize = 0;
    STARTING_LCN_INPUT_BUFFER  InBuf;
    VOLUME_BITMAP_BUFFER  Buffer;
    VOLUME_BITMAP_BUFFER* Result = &Buffer;

    // открываем диск для работы
    hDrive = CreateFile(drive, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

    if (hDrive == INVALID_HANDLE_VALUE)
    {
        ret = GetLastError();
        return NULL;
    }

    nOutSize = sizeof(VOLUME_BITMAP_BUFFER);
    InBuf.StartingLcn.QuadPart = 0;

    // получаем битовую карту   
    ret = DeviceIoControl(hDrive,
        FSCTL_GET_VOLUME_BITMAP,
        &InBuf,
        sizeof(InBuf),
        Result,
        nOutSize,
        &Bytes,
        NULL);

    // если произошла ошибка при получении данных
    if (!ret && GetLastError() == ERROR_MORE_DATA)
    {
        // получаем кол-во кластеров на томе (начиная с StartingLcn)
        _int64 CountClusters = Result->BitmapSize.QuadPart - Result->StartingLcn.QuadPart;
        // вычисляем, сколько нужно байт под буфер (1 кластер = 1 бит)
        nOutSize = CountClusters / sizeof(char) + sizeof(VOLUME_BITMAP_BUFFER);
        Result = (PVOLUME_BITMAP_BUFFER)new char[nOutSize];

        if (!Result)
            return NULL;
        Result->StartingLcn.QuadPart = 0;

        // получаем битовую маску
        ret = DeviceIoControl(hDrive,
            FSCTL_GET_VOLUME_BITMAP,
            &InBuf,
            sizeof(InBuf),
            Result,
            nOutSize,
            &Bytes,
            NULL);

        if (ret)
        {
            CloseHandle(hDrive);
            return Result;
        }
        else
            ret = GetLastError();
    }

    CloseHandle(hDrive);

    return 0;
}

// функция получения таблицы размещения файла на диске
RETRIEVAL_POINTERS_BUFFER* readFileBitmap(std::wstring fileName)
{
    HANDLE hFile;
    STARTING_VCN_INPUT_BUFFER startingVcn = { 0 };
    RETRIEVAL_POINTERS_BUFFER* fileBitmap;
    UINT32 bitmapSize;
    DWORD bytesReturned;
    int ret;
    std::wstring bitmapFile;

    // подключаемся к файлу
    hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        ret = GetLastError();

    startingVcn.StartingVcn.QuadPart = 0;
    bitmapSize = 32 * 1024 + sizeof(LARGE_INTEGER) * 2;
    fileBitmap = (RETRIEVAL_POINTERS_BUFFER*)malloc(bitmapSize);

    char* pBuf = new char[1024 * 2];
    ZeroMemory(pBuf, 1024 * 2);
    DWORD nOutBufferSize = 1024 * 2;

    while (TRUE) {
        // получаем размещение файла на кластерах диска
        ret = DeviceIoControl(hFile,
            FSCTL_GET_RETRIEVAL_POINTERS,
            &startingVcn,
            sizeof(STARTING_VCN_INPUT_BUFFER),
            fileBitmap,
            bitmapSize,
            &bytesReturned,
            NULL);

        if (FALSE == ret) {
            if (GetLastError() != ERROR_MORE_DATA) {
                delete[] pBuf;
                free(fileBitmap);
                CloseHandle(hFile);
                return NULL;
            }
        }

        bytesReturned -= sizeof(LARGE_INTEGER) * 2;
        if (ret) break;
        startingVcn.StartingVcn.QuadPart += bytesReturned * 8;
    }

    delete[] pBuf;

    CloseHandle(hFile);

    return fileBitmap;
}

// функция перемещения частей файла на диске
int Move(LPCWSTR lpSrcName, LPCWSTR drive)
{
    int ret = 0;
    ULONG nClusterSize = 0;
    ULONG nBlockSize = 0;
    ULONG nClCount = 0;
    ULONG nFileSize = 0;
    ULONG nBytes = 0;
    HANDLE hDrive = 0;
    HANDLE hFile = 0;
    ULONG nSecPerCl, nBtPerSec;
    MOVE_FILE_DATA InBuffer;
    ULONG nOutSize = 0;
    LPDWORD pBytes = new DWORD[100];
    ULONG nCls = 0;
    __int64* pClusters = NULL;
    BOOLEAN bResult = FALSE;
    LARGE_INTEGER PrevVCN, LCN;
    VOLUME_BITMAP_BUFFER* pOutBuf = readVolumeBitmap(drive);
    STARTING_VCN_INPUT_BUFFER  InBuf;
    RETRIEVAL_POINTERS_BUFFER* OutBuf = NULL;
    WCHAR* Name = new WCHAR[3];
    int res = 1;

    if (pOutBuf != NULL)
    {
        res = 0;
        // подключаемся к файлу
        hFile = CreateFile(lpSrcName,
            FILE_READ_ATTRIBUTES,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL, OPEN_EXISTING,
            0,
            0);

        Name[0] = lpSrcName[0];
        Name[1] = ':';
        Name[2] = 0;

        // получаем размер свободного места на диске
        if (GetDiskFreeSpace((LPWSTR)Name, &nSecPerCl, &nBtPerSec, NULL, NULL) == FALSE)
        {
            ret = GetLastError();

            delete[] pBytes;
            delete[] Name;
            delete[] pOutBuf;

            CloseHandle(hFile);

            return ret;
        }

        // получаем более точную таблицу размещения файла на диске
        // и считаем кластеры файла
        nClusterSize = nSecPerCl * nBtPerSec;
        if (hFile != INVALID_HANDLE_VALUE)
        {
            nFileSize = GetFileSize(hFile, NULL);
            nOutSize = sizeof(RETRIEVAL_POINTERS_BUFFER) + (nFileSize / nClusterSize) * sizeof(OutBuf->Extents);
            OutBuf = new RETRIEVAL_POINTERS_BUFFER[nOutSize];
            InBuf.StartingVcn.QuadPart = 0;

            if (DeviceIoControl(hFile,
                FSCTL_GET_RETRIEVAL_POINTERS,
                &InBuf,
                sizeof(InBuf),
                OutBuf,
                nOutSize,
                pBytes,
                NULL))
            {
                nClCount = (nFileSize + nClusterSize - 1) / nClusterSize;
                pClusters = new _int64[nClCount];
                PrevVCN = OutBuf->StartingVcn;
                for (ULONG i = 0, Cls = 0; i < OutBuf->ExtentCount; i++)
                {
                    LCN = OutBuf->Extents[i].Lcn;

                    for (int j = (ULONG)(OutBuf->Extents[i].NextVcn.QuadPart - PrevVCN.QuadPart); j > 0; j--, nCls++, LCN.QuadPart++)
                    {
                        *(pClusters + Cls) = LCN.QuadPart;
                    }

                    PrevVCN = OutBuf->Extents[i].NextVcn;
                }
            }

        }
        CloseHandle(hFile);

        // подключаемся к файлу
        hFile = CreateFile(lpSrcName,
            FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_NO_BUFFERING,
            NULL);

        // подключаемся к диску
        hDrive = CreateFile(drive,
            GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING,
            0,
            0);

        if (pClusters)
        {
            // анализируем битовую таблицу диска и ищем достаточно последовательного свободного места
            // для размещения файла
            LONGLONG nStartLCN(0), nEmptyCluster(0), nHelpLCN(0), nMask(1), nInUse(0);
            for (__int64 i = 0; i < pOutBuf->BitmapSize.QuadPart; i++)
            {
                if (pOutBuf->Buffer[i] == 255)
                {
                    nStartLCN += 8;
                    nEmptyCluster = 0;
                    continue;
                }

                while (nMask != 256)
                {
                    if (!nEmptyCluster)
                        nHelpLCN = nStartLCN;

                    nInUse = pOutBuf->Buffer[i] & nMask;

                    if (!nInUse)
                        nEmptyCluster++;
                    else
                        nEmptyCluster = 0;

                    nMask <<= 1;
                    nStartLCN++;
                }

                if (nEmptyCluster >= nClCount)
                {
                    nStartLCN = nHelpLCN;
                    break;
                }

                nMask = 1;
            }

            // перемечаем части файла
            PrevVCN.QuadPart = 0;
            InBuffer.FileHandle = hFile;
            InBuffer.StartingLcn.QuadPart = nStartLCN;

            for (int k = 0; k < OutBuf->ExtentCount; k++)
            {
                InBuffer.StartingVcn = PrevVCN;
                InBuffer.ClusterCount = OutBuf->Extents[k].NextVcn.QuadPart - PrevVCN.QuadPart;

                if (DeviceIoControl(hDrive,
                    FSCTL_MOVE_FILE,
                    &InBuffer,
                    sizeof(InBuffer),
                    NULL,
                    NULL,
                    &nBytes,
                    NULL) == FALSE)
                {
                    ret = GetLastError();

                    delete[] pBytes;
                    delete[] Name;
                    delete[] pOutBuf;
                    delete[] OutBuf;
                    delete[] pClusters;

                    CloseHandle(hFile);
                    CloseHandle(hDrive);

                    return ret;
                }

                InBuffer.StartingLcn.QuadPart += InBuffer.ClusterCount;
                PrevVCN = OutBuf->Extents[k].NextVcn;
            }
        }
    }

    delete[] pBytes;
    delete[] Name;
    delete[] OutBuf;
    delete[] pOutBuf;
    delete[] pClusters;

    CloseHandle(hDrive);
    CloseHandle(hFile);

    return res;
}

std::vector<DefragmentationLogItem*> __cdecl getDefragmentationLogs() 
{
    std::vector<DefragmentationLogItem*> result;
    /*TryEnterCriticalSection(&criticalSection);
    result = DefragmentationLogs;
    DefragmentationLogs.clear();
    LeaveCriticalSection(&criticalSection);*/
    return result;
}

void createLog(const wchar_t* res, ATL::CString fullName)
{
    /*TryEnterCriticalSection(&criticalSection);
    DefragmentationLogItem* item = new DefragmentationLogItem();

    std::wostringstream wss;
    wss << res;
    wss.str().copy(item->result, 1, 0);
    wss.str(std::wstring());

    wss << fullName;
    wss.str().copy(item->fullName, fullName.GetLength(), 0);
    wss.str(std::wstring());

    DefragmentationLogs.push_back(item);
    LeaveCriticalSection(&criticalSection);*/
}


std::queue<DefragmentationLogItem*> __cdecl getTestDefragmentationLogs()
{
    std::queue<DefragmentationLogItem*> testLog_ret;
    if (TryEnterCriticalSection(&criticalSection)) {
        int size = testLog.size();
        for (int i = 0; i < size; i++) {
            testLog_ret.push(testLog.front());
            testLog.pop();
        }
        LeaveCriticalSection(&criticalSection);
    }
    return testLog_ret;
   
}

DWORD WINAPI Defragmentation(LPVOID t) {
    createTestLog();
    return 0;
}

void createTestLog()
{
    for (int j = 0; j < 1000000000; j++) {
        if (TryEnterCriticalSection(&criticalSection)) {

            for (int i = 0; i < 10; i++) {
                DefragmentationLogItem* item = new DefragmentationLogItem();
                //ZeroMemory(item, sizeof(item));

                std::wostringstream wss;
                wss << "=";
                wss.str().copy(item->result, 5, 0);
                wss.str(std::wstring());

                wss << directories[i];
                wss.str().copy(item->fullName, 200, 0);
                wss.str(std::wstring());

                testLog.push(item);
            }
            LeaveCriticalSection(&criticalSection);
        }
        Sleep(500);
        
    }
    int i = 10;
         
}

void __cdecl InitCS() {
    InitializeCriticalSection(&criticalSection);
}

void __cdecl DeleteCS() {
    DeleteCriticalSection(&criticalSection);
}
