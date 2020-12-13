//5x5固定垂直方向网格线
void elastic_mesh::GrayDensityAndEdge3()
{
	// TODO: 在此处添加实现代码.
	int a = 0;
	int b = 1;
	int i, j;
	int linebyte = (m_imgWidth * m_nBitCount + 31) / 8 / 4 * 4;

	SumD = 0;
	Density = new int *[m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		Density[i] = new int[m_imgWidth];
		for (j = 0; j < m_imgWidth; j++)
		{
			Density[i][j] = 0;
		}
	}
	Edge1 = new BYTE *[m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		Edge1[i] = new BYTE[m_imgWidth];
		for (j = 0; j < m_imgWidth; j++)
		{
			Edge1[i][j] = 0;
		}
	}
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < m_imgWidth; j++)
		{
			BYTE nowbit = *(m_pImgData + (m_imgHeight - 1 - i) * linebyte + j);
			if (i == 0 || j == 0 || i == m_imgHeight - 1 || j == m_imgWidth - 1)
			{
				if (nowbit < TH)
				{
					Density[i][j] = 1;
					Edge1[i][j] = 1;
					SumD += 1;
				}
			}
			else
			{
				if (nowbit < TH)
				{
					Density[i][j] = 1;
					SumD += 1;
					BYTE nowbitT = *(m_pImgData + (m_imgHeight - 1 - (i - 1)) * linebyte + j);
					BYTE nowbitB = *(m_pImgData + (m_imgHeight - 1 - (i + 1)) * linebyte + j);
					BYTE nowbitL = *(m_pImgData + (m_imgHeight - 1 - i) * linebyte + (j - 1));
					BYTE nowbitR = *(m_pImgData + (m_imgHeight - 1 - i) * linebyte + (j + 1));
					BYTE nowbitLT = *(m_pImgData + (m_imgHeight - 1 - (i - 1)) * linebyte + (j - 1));
					BYTE nowbitRT = *(m_pImgData + (m_imgHeight - 1 - (i - 1)) * linebyte + (j + 1));
					BYTE nowbitLB = *(m_pImgData + (m_imgHeight - 1 - (i + 1)) * linebyte + (j - 1));
					BYTE nowbitRB = *(m_pImgData + (m_imgHeight - 1 - (i + 1)) * linebyte + (j + 1));
					if (nowbitT >= TH || nowbitB >= TH || nowbitL >= TH || nowbitR >= TH || nowbitLT >= TH || nowbitRT >= TH || nowbitLB >= TH || nowbitRB >= TH)
					{
						Edge1[i][j] = 1;
					}
				}
			}
		}
	}

	//下面按密度把图像划分成5*5网格，格线分别记录在ShuLine[]（竖直方向的格线）和hengline[][]（水平方向格线）数组中
	for (i = 0; i < 6; i++)
	{
		ShuLine[i] = 0;
	}
	ShuLine[0] = 0;
	ShuLine[5] = m_imgWidth - 1;

	float fiH = 0;
	float fiV = 0;
	//固定竖直方向网格线
	int k = 1;
	for (j = 0; j < m_imgWidth; j++)
	{
		for (i = 0; i < m_imgHeight; i++)
			fiV += Density[i][j];
		if (fiV > (k*SumD / 5.0) && k < 5)
		{
			ShuLine[k] = j;
			k++;
		}
	}
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 5; j++)
		{
			hengline[i][j] = 0;
		}
	}
	for (j = 0; j < 5; j++)
	{
		hengline[5][j] = m_imgHeight - 1;
	}
	for (int z = 0; z < 5; z++)
	{
		k = 1;
		fiH = 0;
		for (i = 0; i < m_imgHeight; i++)
		{
			for (j = ShuLine[z]; j < ShuLine[z + 1]; j++)
			{
				fiH += Density[i][j];
			}
			if (fiH > (k*SumD / 5.0 / 5.0) && k < 5)
			{
				hengline[k][z] = i;
				k++;
			}
		}
	}

	//画轮廓
	int linebyteOut1 = (m_imgWidth*m_nBitCount + 31) / 8 / 4 * 4;
	m_pImgDataOut1 = new unsigned char[linebyteOut1*m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < linebyteOut1; j++)
		{
			*(m_pImgDataOut1 + i * linebyteOut1 + j) = 255;
		}
	}
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < m_imgWidth; j++)
		{
			if (Edge1[m_imgHeight - i - 1][j] == 1)
			{
				*(m_pImgDataOut1 + i * linebyteOut1 + j) = 0;
			}
		}
	}
	int linebyteOut = (m_imgWidth * m_nBitCount * 3 + 31) / 8 / 4 * 4;
	m_pImgDataOut = new unsigned char[m_imgHeight * linebyteOut];
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < m_imgWidth; j++)
		{
			BYTE s = *(m_pImgDataOut1 + i * linebyteOut1 + j);
			*(m_pImgDataOut + i * linebyteOut + j * 3 + 0) = s;
			*(m_pImgDataOut + i * linebyteOut + j * 3 + 1) = s;
			*(m_pImgDataOut + i * linebyteOut + j * 3 + 2) = s;
		}
	}
	//画网格线
	for (int t = 0; t < 6; t++)
	{
		for (int z = 0; z < 5; z++)
		{
			for (j = ShuLine[z]; j < ShuLine[z + 1]; j++)
			{
				*(m_pImgDataOut + (m_imgHeight - 1 - hengline[t][z]) * linebyteOut + j * 3 + 0) = 0;
				*(m_pImgDataOut + (m_imgHeight - 1 - hengline[t][z]) * linebyteOut + j * 3 + 1) = 0;
				*(m_pImgDataOut + (m_imgHeight - 1 - hengline[t][z]) * linebyteOut + j * 3 + 2) = 255;
			}
		}
	}
	for (i = 0; i < m_imgHeight; i++)
	{
		for (int z = 0; z < 6; z++)
		{
			*(m_pImgDataOut + (m_imgHeight - 1 - i) * linebyteOut + ShuLine[z] * 3 + 0) = 0;
			*(m_pImgDataOut + (m_imgHeight - 1 - i) * linebyteOut + ShuLine[z] * 3 + 1) = 0;
			*(m_pImgDataOut + (m_imgHeight - 1 - i) * linebyteOut + ShuLine[z] * 3 + 2) = 255;
		}
	}
}


//5x5固定45°网格线
void elastic_mesh::GrayDensityAndEdge5()
{
	// TODO: 在此处添加实现代码.
	int i, j;
	//int SumD = 0;
	int linebyte = (m_imgWidth * m_nBitCount + 31) / 8 / 4 * 4;

	//下面按密度把图像划分成5*5网格，格线分别记录在HengLine1（水平方向的格线）和shuline1（垂直方向格线）数组中
	for (i = 0; i < 6; i++)
	{
		HengLine1[i] = 0;
	}
	HengLine1[0] = 0;
	HengLine1[5] = m_imgHeight + m_imgWidth - 1;

	float fiH = 0;
	float fiV = 0;
	int k = 1;
	//固定水平方向网格线
	for (i = 0; i < m_imgHeight + m_imgWidth - 1; i++)
	{
		for (j = 0; j < m_imgHeight + m_imgWidth - 1; j++)
			fiH += Density1[i][j];
		if (fiH > (k*SumD / 5.0) && k < 5)
		{
			HengLine1[k] = i;
			k++;
		}
	}
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			shuline1[i][j] = 0;
		}
	}
	for (i = 0; i < 5; i++)
	{
		shuline1[i][5] = m_imgHeight + m_imgWidth - 1;
	}
	for (int z = 0; z < 5; z++)
	{
		k = 1;
		fiV = 0;
		for (j = 0; j < m_imgHeight + m_imgWidth - 1; j++)
		{
			for (i = HengLine1[z]; i < HengLine1[z + 1]; i++)
			{
				fiV += Density1[i][j];
			}
			if (fiV > (k*SumD / 5.0 / 5.0) && k < 5)
			{
				shuline1[z][k] = j;
				k++;
			}
		}
	}

	//画网格线
	for (int t = 0; t < 5; t++)
	{
		for (int z = 1; z < 5; z++)
		{
			for (i = HengLine1[t]; i < HengLine1[t + 1]; i++)
			{
				if (Xline[i][shuline1[t][z]] != 222)
				{
					Xline[i][shuline1[t][z]] = 127;
				}
			}
		}
	}
	for (int t = 1; t < 5; t++)
	{
		for (j = 0; j < m_imgHeight + m_imgWidth - 1; j++)
		{
			if (Xline[HengLine1[t]][j] != 222)
			{
				Xline[HengLine1[t]][j] = 127;
			}
		}
	}
	//还原
	for (i = 0; i < m_imgHeight; i++)
	{
		j = 0;
		while (j < m_imgHeight + m_imgWidth - 1)
		{
			BYTE nowbit = Xline[i][j];
			if (nowbit != 222)
			{
				break;
			}
			j++;
		}
		int a = i;
		int b = j + i + 1 - m_imgHeight;
		while (j < m_imgHeight + m_imgWidth - 1 && a < m_imgHeight + m_imgWidth - 1)
		{
			BYTE nowbit1 = Xline[a][j];
			if (nowbit1 == 127)
			{
				*(m_pImgData + (m_imgHeight - 1 - i) * linebyte + b) = 127;
			}
			b++;
			a++;
			j++;
		}
	}

	//轮廓提取
	BYTE** Line = new BYTE *[m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		Line[i] = new BYTE[m_imgWidth];
		for (j = 0; j < m_imgWidth; j++)
		{
			if (*(m_pImgData + (m_imgHeight - 1 - i) * linebyte + j) == 127)
			{
				Line[i][j] = 1;
			}
			else
			{
				Line[i][j] = 0;
			}
		}
	}
	int linebyteOut1 = (m_imgWidth*m_nBitCount + 31) / 8 / 4 * 4;
	m_pImgDataOut1 = new unsigned char[linebyteOut1*m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < linebyteOut1; j++)
		{
			*(m_pImgDataOut1 + i * linebyteOut1 + j) = 255;
		}
	}
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < m_imgWidth; j++)
		{
			if (Line[i][j] == 1)
			{
				*(m_pImgDataOut1 + i * linebyteOut1 + j) = 127;
			}
			else if (Edge1[m_imgHeight - i - 1][j] == 1)
			{
				*(m_pImgDataOut1 + i * linebyteOut1 + j) = 0;
			}
		}
	}
	//转为24位图
	int linebyteOut = (m_imgWidth * m_nBitCount * 3 + 31) / 8 / 4 * 4;
	m_pImgDataOut = new unsigned char[m_imgHeight * linebyteOut];
	for (i = 0; i < m_imgHeight; i++)
	{
		for (j = 0; j < m_imgWidth; j++)
		{
			BYTE s = *(m_pImgDataOut1 + i * linebyteOut1 + j);
			if (s == 127)
			{
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 0) = 0;
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 1) = 0;
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 2) = 255;
			}
			else
			{
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 0) = s;
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 1) = s;
				*(m_pImgDataOut + i * linebyteOut + j * 3 + 2) = s;
			}
		}
	}

	//画边缘网格线
	for (i = 0; i < m_imgHeight; i++)
	{
		*(m_pImgDataOut + i * linebyteOut + 0 * 3 + 0) = 0;
		*(m_pImgDataOut + i * linebyteOut + 0 * 3 + 1) = 0;
		*(m_pImgDataOut + i * linebyteOut + 0 * 3 + 2) = 255;
		*(m_pImgDataOut + i * linebyteOut + (m_imgWidth - 1) * 3 + 0) = 0;
		*(m_pImgDataOut + i * linebyteOut + (m_imgWidth - 1) * 3 + 1) = 0;
		*(m_pImgDataOut + i * linebyteOut + (m_imgWidth - 1) * 3 + 2) = 255;
	}
	for (j = 0; j < m_imgWidth; j++)
	{
		*(m_pImgDataOut + 0 * linebyteOut + j * 3 + 0) = 0;
		*(m_pImgDataOut + 0 * linebyteOut + j * 3 + 1) = 0;
		*(m_pImgDataOut + 0 * linebyteOut + j * 3 + 2) = 255;
		*(m_pImgDataOut + (m_imgHeight - 1) * linebyteOut + j * 3 + 0) = 0;
		*(m_pImgDataOut + (m_imgHeight - 1) * linebyteOut + j * 3 + 1) = 0;
		*(m_pImgDataOut + (m_imgHeight - 1) * linebyteOut + j * 3 + 2) = 255;
	}
}