// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include <cstdlib>
#include <xdebug>

#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <vector>
#include <map>
#include <list>
#include <string>
#include "boost/shared_ptr.hpp"
#include "Utils.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
