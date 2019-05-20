// Reorder chip images

#include "pch.h"

bool StrEqual(const tstring& a, const tstring& b)
{
	size_t sz = a.size();
	if (b.size() != sz)
		return false;
	for (size_t i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

bool ParamExists(vector<tstring> & args, tstring param)
{
	for (auto it = args.begin(); it != args.end(); ++it)
	{
		if (StrEqual(*it, param))
		{
			return true;
		}
	}
	return false;
}

bool GetParam(vector<tstring> & args, tstring whatName, tstring & name)
{
	for (auto it = args.begin(); it != args.end(); ++it)
	{
		if (StrEqual(*it, whatName) && it != args.end())
		{
			it++;
			name = *it;
			return true;
		}
	}

	return false;
}

/// ProgName.exe /inputDir <path> /outpuDir <path> /cols <num> /rows <num> /method {RotateRight|RotateLeft|FlipHorz|FlipVert}

/// Apply method on files cXXXX_rYYYY in input dir and put result in output dir


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR inputFile[0x1000];
	TCHAR outputFile[0x1000];
	vector<tstring> args;

	tcout << _T("reorder - Apply method on files cXXXX_rYYYY in input dir and put result in output dir") << endl;

	for (int i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	/// Parse parameters

	tstring cols;
	if (!GetParam(args, _T("/cols"), cols))
	{
		tcout << "Specify /cols number" << endl;
		return -1000;
	}
	int columnMax = _tstoi(cols.c_str());

	tstring rows;
	if (!GetParam(args, _T("/rows"), rows))
	{
		tcout << "Specify /rows number" << endl;
		return -1001;
	}
	int rowMax = _tstoi(rows.c_str());

	tstring inputDir;
	if (!GetParam(args, _T("/inputDir"), inputDir))
	{
		tcout << "Specify /inputDir path" << endl;
		return -1002;
	}

	tstring outputDir;
	if (!GetParam(args, _T("/outputDir"), outputDir))
	{
		tcout << "Specify /outputDir path" << endl;
		return -1003;
	}

	tstring method;
	if (!GetParam(args, _T("/method"), method))
	{
		tcout << "Specify /method RotateRight RotateLeft FlipHorz FlipVert" << endl;
		return -1004;
	}

	/// Do reorder

	for (int column = 0; column < columnMax; column++)
	{
		for (int row = 0; row < rowMax; row++)
		{
			/// TODO: Method

			_stprintf_s(inputFile, _countof(inputFile), _T("%s\\c%04i_r%04i.jpg"), inputDir.c_str(), column, rowMax - (row + 1));
			_stprintf_s(outputFile, _countof(outputFile), _T("%s\\c%04i_r%04i.jpg"), outputDir.c_str(), column, row);

			CopyFile(inputFile, outputFile, FALSE);
		}
	}

}
