// Registry.cpp : implementation file
//

#include "Registry.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// CRegistry

CRegistry::CRegistry(HKEY hKey)
{
	m_hKey=hKey;
	m_hKeyForEnum = nullptr;
    m_dwIndex = 0;
}

CRegistry::~CRegistry()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegistry Functions

BOOL CRegistry::CreateKey(LPCTSTR lpSubKey)
{
	assert(m_hKey);
	assert(lpSubKey);

	HKEY hKey;
	DWORD dw;
	long lReturn=RegCreateKeyEx(m_hKey,lpSubKey,0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dw);
    
	if(lReturn==ERROR_SUCCESS)
	{
		m_hKey = hKey;
        m_dwIndex = 0;
		return TRUE;
	}
	
	return FALSE;
	
}

BOOL CRegistry::Open(LPCTSTR lpSubKey)
{
	assert(m_hKey);
	assert(lpSubKey);
    
	HKEY hKey;
	long lReturn=RegOpenKeyEx(m_hKey,lpSubKey,0L,KEY_ALL_ACCESS,&hKey);
    
	if(lReturn==ERROR_SUCCESS)
	{
        m_hKey = hKey;
        m_dwIndex = 0;
		return TRUE;
	}
	return FALSE;
	
}

void CRegistry::Close()
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey=NULL;
	}
	
}

BOOL CRegistry::DeleteValue(LPCTSTR lpValueName)
{
	assert(m_hKey);
	assert(lpValueName);
	
	long lReturn=RegDeleteValue(m_hKey,lpValueName);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
	assert(hKey);
	assert(lpSubKey);
	
	long lReturn=RegDeleteValue(hKey,lpSubKey);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	return FALSE;
	
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, int nVal)
{
	assert(m_hKey);
	assert(lpSubKey);
	
	DWORD dwValue;
	dwValue=(DWORD)nVal;
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, DWORD dwVal)
{
	assert(m_hKey);
	assert(lpSubKey);
	
	long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}

BOOL CRegistry::Write(LPCTSTR lpValueName, LPCTSTR lpValue)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(lpValue);	

	long lReturn=RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ,(const BYTE *) lpValue, wcslen(lpValue)+1);
	
   	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
	
}


BOOL CRegistry::Read(LPCTSTR lpValueName, int* pnVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(pnVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pnVal=(int)dwDest;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistry::Read(LPCTSTR lpValueName, DWORD* pdwVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(pdwVal);
	
	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*pdwVal=dwDest;
		return TRUE;
	}
	return FALSE;
	
}




BOOL CRegistry::RestoreKey(LPCTSTR lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);
	
	long lReturn=RegRestoreKey(m_hKey,lpFileName,REG_WHOLE_HIVE_VOLATILE);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::SaveKey(LPCTSTR lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);
	
	long lReturn=RegSaveKey(m_hKey,lpFileName,NULL);
	
	if(lReturn==ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}


BOOL CRegistry::Read(LPCTSTR lpValueName, string* lpVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(lpVal);
	
	DWORD dwType;
	DWORD dwSize=200;
	char szString[2550] = {0};
	
	long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)szString,&dwSize);
	
	if(lReturn==ERROR_SUCCESS)
	{
		*lpVal=szString;
		return TRUE;
	}
	return FALSE;
	
}

BOOL CRegistry::EnumRead( LPCTSTR lpValueName, string* lpVal )
{
    assert(lpVal);

    string sSubKey;
    if (GetSubKey(&sSubKey)){
		if (ERROR_SUCCESS == RegOpenKeyExA(m_hKey, sSubKey.c_str(), 0L, KEY_ALL_ACCESS, &m_hKeyForEnum)){
            char szValue[MAX_PATH];
            DWORD dwSize = MAX_PATH;
            DWORD dwType;
			if (ERROR_SUCCESS == RegQueryValueEx(m_hKeyForEnum, lpValueName, NULL, &dwType, (BYTE *)szValue, &dwSize)){
                *lpVal = szValue;
            }
        }
        return TRUE;
    }else{
        return FALSE;
    }
}

BOOL CRegistry::EnumRead2(LPCTSTR lpValueName, string* lpVal)
{
	char szValue[MAX_PATH];
	DWORD dwSize = MAX_PATH;
	DWORD dwType;
	if (ERROR_SUCCESS == RegQueryValueEx(m_hKeyForEnum, lpValueName, NULL, &dwType, (BYTE *)szValue, &dwSize)){
		*lpVal = szValue;
		return TRUE;
	}
	else{
		return FALSE;
	}
}


BOOL CRegistry::GetSubKey( string* lpVal )
{
    assert(lpVal);

    char szKeyName[MAX_PATH];
    DWORD dwKeyLen = MAX_PATH;
    *lpVal = "";
    if(ERROR_SUCCESS == RegEnumKeyExA(m_hKey,m_dwIndex,szKeyName,&dwKeyLen,0,NULL,NULL,NULL)){
        m_dwIndex ++;
        *lpVal = szKeyName;
        return TRUE;
    }else{
        m_dwIndex = 0;
        return FALSE;
    }
}

