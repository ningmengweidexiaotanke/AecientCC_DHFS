void index::DHFSFeature(CString filename)
{
	// TODO: 在此处添加实现代码.
	int I, J, I1, J1;
	//本次处理的图像大小均在100到130之间，所以数组多用了静态数组，大小为130如有大于130的将忽略超出的部分 
	I = m_imgHeight;
	J = m_imgWidth;
	I1 = m_imgHeight + m_imgWidth - 1;
	J1 = m_imgHeight + m_imgWidth - 1;
	int linebyte = (m_imgWidth * m_nBitCount + 31) / 8 / 4 * 4;


	//设置方向像素数组，保存求出的各像素的方向线素值，在此基础上再统计各网格的特征向量。
	//float H[130][130];
	//float S[130][130];
	//float P[259][259];//130 + 130 - 1
	//float N[259][259];
	int i, j, k, t;
	float **H = new float*[m_imgHeight];
	float **S = new float*[m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		H[i] = new float[m_imgWidth];
		S[i] = new float[m_imgWidth];
		for (j = 0; j < m_imgWidth; j++)
		{
			H[i][j] = 0;
			S[i][j] = 0;
		}
	}
	float **P = new float*[m_imgHeight + m_imgWidth + 1];
	float **N = new float*[m_imgHeight + m_imgWidth + 1];
	for (i = 0; i < m_imgHeight + m_imgWidth + 1; i++)
	{
		P[i] = new float[m_imgHeight + m_imgWidth + 1];
		N[i] = new float[m_imgHeight + m_imgWidth + 1];
		for (j = 0; j < m_imgHeight + m_imgWidth + 1; j++)
		{
			P[i][j] = 0;
			N[i][j] = 0;
		}
	}

	//for (i = 0; i < I; i++)
	//	for (j = 0; j < J; j++)
	//	{
	//		H[i][j] = 0;
	//		S[i][j] = 0;
	//	}
	//for (i = 0; i < I1; i++)
	//{
	//	for (j = 0; j < J1; j++)
	//	{
	//		P[i][j] = 0;
	//		N[i][j] = 0;
	//	}
	//}
	//////////////////////////////////////////////////
	//方向线素特征
	float SumH = 0;
	float SumS = 0;
	float SumP = 0;
	float SumN = 0;
	for (i = 1; i < I - 1; i++)
		for (j = 1; j < J - 1; j++)
		{
			if (Edge1[i][j] == 0)
				continue;
			if (Edge1[i][j + 1] == 1)
			{
				H[i][j]++;
				SumH++;
			}
			if (Edge1[i + 1][j] == 1)
			{
				S[i][j]++;
				SumS++;
			}
		}
	for (i = 2; i < I1 - 2; i++)
		for (j = 2; j < J1 - 2; j++)
		{
			if (XEdge1[i][j] == 0 || XEdge1[i][j] == 2)
				continue;
			if (XEdge1[i][j + 1] == 2 && XEdge1[i][j + 2] == 1)
			{
				P[i][j]++;
				SumP++;
			}
			if (XEdge1[i + 1][j] == 2 && XEdge1[i + 2][j] == 1)
			{
				N[i][j]++;
				SumN++;
			}
		}


	float feature[100];
	for (i = 0; i < 100; i++)
		feature[i] = 0;
	int kuai = 0;
	int endh, endl;
	//按块计算各块的特征按H\S\P\N的次序存放
	//横笔画特征()固定垂直网格线
	int temp_feature[25];
	for (i = 0; i < 25; i++)
	{
		temp_feature[i] = 0;
	}
	int temp_f = 0;
	for (t = 0; t < 5; t++)
	{
		for (k = 0; k < 5; k++)
		{
			temp_f = 0;
			kuai = t * 5 + k;
			for (j = ShuLine[t]; j < ShuLine[t + 1]; j++)//列
			{
				for (i = hengline[k][t]; i < hengline[k + 1][t]; i++)//行
				{
					temp_f += H[i][j];
				}
			}
			temp_feature[kuai] = temp_f;
		}
	}
	float c_c_1 = 0.;//左上
	float c_c_2 = 0.;//左
	float c_c_3 = 0.;//左下
	float c_c_4 = 0.;//右上
	float c_c_5 = 0.;//右
	float c_c_6 = 0.;//右下
	for (t = 0; t < 5; t++)//列
	{
		for (k = 0; k < 5; k++)//行
		{
			int temp_pixel_num = 0;
			int stroke_num1 = 0;
			int stroke_num2 = 0;
			//左上网格
			if (t == 0 || k == 0)
			{
				pixel_index_1_1 = 0.;
				pixel_index_1_2 = 0.;
				stroke_index_1_1 = 0.;
				stroke_index_1_2 = 0.;
				grid_index_1_1 = 0.;
				grid_index_1_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_1_1 = 0.;
				pixel_index_1_2 = 0.;
				for (j = ShuLine[t - 1]; j < ShuLine[t]; j++)
				{
					for (i = hengline[k - 1][t - 1]; i < hengline[k][t - 1]; i++)
					{
						if (H[i][j] > 0)
						{
							temp_pixel_num += 1;
							pixel_index_1_1 += exp(-(pow(3 * (ShuLine[t] - j) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
							pixel_index_1_2 += exp(-(pow(3 * (j - ShuLine[t - 1]) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
						}
					}
				}
				pixel_index_1_1 = (pixel_index_1_1 / temp_pixel_num) - 0.1;
				pixel_index_1_2 = (pixel_index_1_2 / temp_pixel_num) - 0.1;
				//笔画位置指标
				stroke_index_1_1 = 0.;
				stroke_index_1_2 = 0.;
				for (i = hengline[k - 1][t - 1]; i < hengline[k][t - 1]; i++)
				{
					for (j = ShuLine[t - 1] + 1; j <= ShuLine[t]; j++)
					{
						if (j == ShuLine[t] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_1_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_1_2 = 1 - stroke_index_1_1;
				//网格位置指标
				grid_index_1_1 = 0.;
				grid_index_1_2 = 0.;
				if (hengline[k - 1][t - 1] <= hengline[k][t] && (hengline[k][t - 1] >= hengline[k][t] && hengline[k][t - 1] <= hengline[k + 1][t]))
				{
					grid_index_1_1 = (float)(hengline[k][t - 1] - hengline[k][t]) / (hengline[k][t - 1] - hengline[k - 1][t - 1]);
					grid_index_1_2 = 1 - grid_index_1_1;
				}
				else if (hengline[k - 1][t - 1] <= hengline[k][t] && hengline[k][t - 1] > hengline[k + 1][t])
				{
					grid_index_1_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k][t - 1] - hengline[k - 1][t - 1]);
					grid_index_1_2 = 1 - grid_index_1_1;
				}
				else if (hengline[k - 1][t - 1] > hengline[k][t] && hengline[k][t - 1] <= hengline[k + 1][t])
				{
					grid_index_1_1 = 1.;
					grid_index_1_2 = 0.;
				}
				else if ((hengline[k - 1][t - 1] > hengline[k][t] && hengline[k - 1][t - 1] <= hengline[k + 1][t]) && hengline[k][t - 1] >= hengline[k + 1][t])
				{
					grid_index_1_1 = (float)(hengline[k + 1][t] - hengline[k - 1][t - 1]) / (hengline[k][t - 1] - hengline[k - 1][t - 1]);
					grid_index_1_2 = 1 - grid_index_1_1;
				}
				else
				{
					grid_index_1_1 = 0.;
					grid_index_1_2 = 1.;
				}
			}
			//左网格
			if (t == 0)
			{
				pixel_index_2_1 = 0.;
				pixel_index_2_2 = 0.;
				stroke_index_2_1 = 0.;
				stroke_index_2_2 = 0.;
				grid_index_2_1 = 0.;
				grid_index_2_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_2_1 = 0.;
				pixel_index_2_2 = 0.;
				temp_pixel_num = 0;
				for (i = hengline[k][t - 1]; i < hengline[k + 1][t - 1]; i++)
				{
					for (j = ShuLine[t - 1]; j < ShuLine[t]; j++)
					{
						if (H[i][j] == 1)
						{
							temp_pixel_num += 1;
							pixel_index_2_1 += exp(-(pow(3 * (ShuLine[t] - j) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
							pixel_index_2_2 += exp(-(pow(3 * (j - ShuLine[t - 1]) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
						}
					}
				}
				pixel_index_2_1 = pixel_index_2_1 / temp_pixel_num - 0.1;
				pixel_index_2_2 = pixel_index_2_2 / temp_pixel_num - 0.1;
				//笔画位置指标
				stroke_index_2_1 = 0.;
				stroke_index_2_2 = 0.;
				for (i = hengline[k][t - 1]; i < hengline[k + 1][t - 1]; i++)
				{
					for (j = ShuLine[t - 1] + 1; j <= ShuLine[t]; j++)
					{
						if (j == ShuLine[t] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_2_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_2_2 = 1 - stroke_index_2_1;
				//网格位置指标
				grid_index_2_1 = 0.;
				grid_index_2_2 = 0.;
				if (hengline[k][t - 1] <= hengline[k + 1][t] && (hengline[k + 1][t - 1] >= hengline[k][t] && hengline[k + 1][t - 1] <= hengline[k + 1][t]))
				{
					grid_index_2_1 = (float)(hengline[k + 1][t - 1] - hengline[k][t]) / (hengline[k + 1][t - 1] - hengline[k][t - 1]);
					grid_index_2_2 = 1 - grid_index_2_1;
				}
				else if (hengline[k][t - 1] <= hengline[k][t] && hengline[k + 1][t - 1] > hengline[k + 1][t])
				{
					grid_index_2_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 1][t - 1] - hengline[k][t - 1]);
					grid_index_2_2 = 1 - grid_index_2_1;
				}
				else if (hengline[k][t - 1] > hengline[k][t] && hengline[k + 1][t - 1] <= hengline[k + 1][t])
				{
					grid_index_2_1 = 1.;
					grid_index_2_2 = 0.;
				}
				else if ((hengline[k][t - 1] > hengline[k][t] && hengline[k][t - 1] <= hengline[k + 1][t]) && hengline[k + 1][t - 1] >= hengline[k + 1][t])
				{
					grid_index_2_1 = (float)(hengline[k + 1][t] - hengline[k][t - 1]) / (hengline[k + 1][t - 1] - hengline[k][t - 1]);
					grid_index_2_2 = 1 - grid_index_2_1;
				}
				else
				{
					grid_index_2_1 = 0.;
					grid_index_2_2 = 1.;
				}
			}
			//左下网格
			if (t == 0 || k == 4)
			{
				pixel_index_3_1 = 0.;
				pixel_index_3_2 = 0.;
				stroke_index_3_1 = 0.;
				stroke_index_3_2 = 0.;
				grid_index_3_1 = 0.;
				grid_index_3_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_3_1 = 0.;
				pixel_index_3_2 = 0.;
				temp_pixel_num = 0;
				for (i = hengline[k + 1][t - 1]; i < hengline[k + 2][t - 1]; i++)
				{
					for (j = ShuLine[t - 1]; j < ShuLine[t]; j++)
					{
						if (H[i][j] == 1)
						{
							temp_pixel_num += 1;
							pixel_index_3_1 += exp(-(pow(3 * (ShuLine[t] - j) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
							pixel_index_3_2 += exp(-(pow(3 * (j - ShuLine[t - 1]) / (ShuLine[t] - ShuLine[t - 1]), 2) / 2));
						}
					}
				}
				pixel_index_3_1 = pixel_index_3_1 / temp_pixel_num - 0.1;
				pixel_index_3_2 = pixel_index_3_2 / temp_pixel_num - 0.1;
				//笔画位置指标
				stroke_index_3_1 = 0.;
				stroke_index_3_2 = 0.;
				for (i = hengline[k + 1][t - 1]; i < hengline[k + 2][t - 1]; i++)
				{
					for (j = ShuLine[t - 1] + 1; j <= ShuLine[t]; j++)
					{
						if (j == ShuLine[t] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_3_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_3_2 = 1 - stroke_index_3_1;
				//网格位置指标
				grid_index_3_1 = 0.;
				grid_index_3_2 = 0.;
				if (hengline[k + 1][t - 1] <= hengline[k + 2][t] && (hengline[k + 2][t - 1] >= hengline[k][t] && hengline[k + 2][t - 1] <= hengline[k + 1][t]))
				{
					grid_index_3_1 = (float)(hengline[k + 2][t - 1] - hengline[k][t]) / (hengline[k + 2][t - 1] - hengline[k + 1][t - 1]);
					grid_index_3_2 = 1 - grid_index_3_1;
				}
				else if (hengline[k + 1][t - 1] <= hengline[k][t] && hengline[k + 2][t - 1] > hengline[k + 1][t])
				{
					grid_index_3_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 2][t - 1] - hengline[k + 1][t - 1]);
					grid_index_3_2 = 1 - grid_index_3_1;
				}
				else if (hengline[k + 1][t - 1] > hengline[k][t] && hengline[k + 2][t - 1] <= hengline[k + 1][t])
				{
					grid_index_3_1 = 1.;
					grid_index_3_2 = 0.;
				}
				else if ((hengline[k + 1][t - 1] > hengline[k][t] && hengline[k + 1][t - 1] <= hengline[k + 1][t]) && hengline[k + 2][t - 1] >= hengline[k + 1][t])
				{
					grid_index_3_1 = (float)(hengline[k + 1][t] - hengline[k + 1][t - 1]) / (hengline[k + 2][t - 1] - hengline[k + 1][t - 1]);
					grid_index_3_2 = 1 - grid_index_3_1;
				}
				else
				{
					grid_index_3_1 = 0.;
					grid_index_3_2 = 1.;
				}
			}
			//右上网格
			if (t == 4 || k == 0)
			{
				pixel_index_4_1 = 0.;
				pixel_index_4_2 = 0.;
				stroke_index_4_1 = 0.;
				stroke_index_4_2 = 0.;
				grid_index_4_1 = 0.;
				grid_index_4_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_4_1 = 0.;
				pixel_index_4_2 = 0.;
				temp_pixel_num = 0;
				for (j = ShuLine[t + 1]; j < ShuLine[t + 2]; j++)
				{
					for (i = hengline[k - 1][t + 1]; i < hengline[k][t + 1]; i++)
					{
						if (H[i][j] > 0)
						{
							temp_pixel_num += 1;
							pixel_index_4_1 += exp(-(pow(3 * (j - ShuLine[t + 1]) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
							pixel_index_4_2 += exp(-(pow(3 * (ShuLine[t + 2] - j) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
						}
					}
				}
				pixel_index_4_1 = pixel_index_4_1 / temp_pixel_num - 0.1;
				pixel_index_4_2 = pixel_index_4_2 / temp_pixel_num - 0.1;
				//笔画位置指标
				stroke_index_4_1 = 0.;
				stroke_index_4_2 = 0.;
				for (i = hengline[k - 1][t + 1]; i < hengline[k][t + 1]; i++)
				{
					for (j = ShuLine[t + 1] + 1; j <= ShuLine[t + 2]; j++)
					{
						if (j == ShuLine[t + 2] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_4_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_4_2 = 1 - stroke_index_4_1;
				//网格位置指标
				grid_index_4_1 = 0.;
				grid_index_4_2 = 0.;
				if (hengline[k - 1][t + 1] <= hengline[k][t] && (hengline[k][t + 1] >= hengline[k][t] && hengline[k][t + 1] <= hengline[k + 1][t]))
				{
					grid_index_4_1 = (float)(hengline[k][t + 1] - hengline[k][t]) / (hengline[k][t + 1] - hengline[k - 1][t + 1]);
					grid_index_4_2 = 1 - grid_index_4_1;
				}
				else if (hengline[k - 1][t + 1] <= hengline[k][t] && hengline[k][t + 1] > hengline[k + 1][t])
				{
					grid_index_4_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k][t + 1] - hengline[k - 1][t + 1]);
					grid_index_4_2 = 1 - grid_index_4_1;
				}
				else if (hengline[k - 1][t + 1] > hengline[k][t] && hengline[k][t + 1] <= hengline[k + 1][t])
				{
					grid_index_4_1 = 1.;
					grid_index_4_2 = 0.;
				}
				else if ((hengline[k - 1][t + 1] > hengline[k][t] && hengline[k - 1][t + 1] <= hengline[k + 1][t]) && hengline[k][t + 1] >= hengline[k + 1][t])
				{
					grid_index_4_1 = (float)(hengline[k + 1][t] - hengline[k - 1][t + 1]) / (hengline[k][t + 1] - hengline[k - 1][t + 1]);
					grid_index_4_2 = 1 - grid_index_4_1;
				}
				else
				{
					grid_index_4_1 = 0.;
					grid_index_4_2 = 1.;
				}
			}
			//右网格
			if (t == 4)
			{
				pixel_index_5_1 = 0.;
				pixel_index_5_2 = 0.;
				stroke_index_5_1 = 0.;
				stroke_index_5_2 = 0.;
				grid_index_5_1 = 0.;
				grid_index_5_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_5_1 = 0.;
				pixel_index_5_2 = 0.;
				temp_pixel_num = 0;
				for (j = ShuLine[t + 1]; j < ShuLine[t + 2]; j++)
				{
					for (i = hengline[k][t + 1]; i < hengline[k + 1][t + 1]; i++)
					{
						if (H[i][j] > 0)
						{
							temp_pixel_num += 1;
							pixel_index_5_1 += exp(-(pow(3 * (j - ShuLine[t + 1]) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
							pixel_index_5_2 += exp(-(pow(3 * (ShuLine[t + 2] - j) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
						}
					}
				}
				pixel_index_5_1 = pixel_index_5_1 / temp_pixel_num - 0.1;
				pixel_index_5_2 = pixel_index_5_2 / temp_pixel_num - 0.1;
				//笔画位置指标
				stroke_index_5_1 = 0.;
				stroke_index_5_2 = 0.;
				for (i = hengline[k][t + 1]; i < hengline[k + 1][t + 1]; i++)
				{
					for (j = ShuLine[t + 1] + 1; j <= ShuLine[t + 2]; j++)
					{
						if (j == ShuLine[t + 2] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_5_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_5_2 = 1 - stroke_index_5_1;
				//网格位置指标
				grid_index_5_1 = 0.;
				grid_index_5_2 = 0.;
				if (hengline[k][t + 1] <= hengline[k][t] && (hengline[k + 1][t + 1] >= hengline[k][t] && hengline[k + 1][t + 1] <= hengline[k + 1][t]))
				{
					grid_index_5_1 = (float)(hengline[k + 1][t + 1] - hengline[k][t]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
					grid_index_5_2 = 1 - grid_index_5_1;
				}
				else if (hengline[k][t + 1] <= hengline[k][t] && hengline[k + 1][t + 1] > hengline[k + 1][t])
				{
					grid_index_5_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
					grid_index_5_2 = 1 - grid_index_5_1;
				}
				else if (hengline[k][t + 1] > hengline[k][t] && hengline[k + 1][t + 1] <= hengline[k + 1][t])
				{
					grid_index_5_1 = 1.;
					grid_index_5_2 = 0.;
				}
				else if ((hengline[k][t + 1] > hengline[k][t] && hengline[k][t + 1] <= hengline[k + 1][t]) && hengline[k + 1][t + 1] >= hengline[k + 1][t])
				{
					grid_index_5_1 = (float)(hengline[k + 1][t] - hengline[k][t + 1]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
					grid_index_5_2 = 1 - grid_index_5_1;
				}
				else
				{
					grid_index_5_1 = 0.;
					grid_index_5_2 = 1.;
				}
			}
			//右下网格
			if (t == 4 || k == 4)
			{
				pixel_index_6_1 = 0.;
				pixel_index_6_2 = 0.;
				stroke_index_6_1 = 0.;
				stroke_index_6_2 = 0.;
				grid_index_6_1 = 0.;
				grid_index_6_2 = 0.;
			}
			else
			{
				//像素距离指标
				pixel_index_6_1 = 0.;
				pixel_index_6_2 = 0.;
				temp_pixel_num = 0;
				for (j = ShuLine[t + 1]; j < ShuLine[t + 2]; j++)
				{
					for (i = hengline[k + 1][t + 1]; i < hengline[k + 2][t + 1]; i++)
					{
						if (H[i][j] > 0)
						{
							temp_pixel_num += 1;
							pixel_index_6_1 += exp(-(pow(3 * (j - ShuLine[t + 1]) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
							pixel_index_6_2 += exp(-(pow(3 * (ShuLine[t + 2] - j) / (ShuLine[t + 2] - ShuLine[t + 1]), 2) / 2));
						}
					}
				}
				pixel_index_6_1 = pixel_index_6_1 / temp_pixel_num - 0.1;
				pixel_index_6_2 = pixel_index_6_2 / temp_pixel_num - 0.1;
				//笔画位置指标
				stroke_index_6_1 = 0.;
				stroke_index_6_2 = 0.;
				for (i = hengline[k + 1][t + 1]; i < hengline[k + 2][t + 1]; i++)
				{
					for (j = ShuLine[t + 1] + 1; j <= ShuLine[t + 2]; j++)
					{
						if (j == ShuLine[t + 2] && H[i][j] == 1)
						{
							stroke_num1 += 1;
						}
						if (H[i][j - 1] == 1 && H[i][j] == 0)
						{
							stroke_num2 += 1;
						}
					}
				}
				stroke_index_6_1 = (float)stroke_num1 / (stroke_num1 + stroke_num2);
				stroke_index_6_2 = 1 - stroke_index_6_1;
				//网格位置指标
				grid_index_6_1 = 0.;
				grid_index_6_2 = 0.;
				if (hengline[k + 1][t + 1] <= hengline[k][t] && (hengline[k + 2][t + 1] >= hengline[k][t] && hengline[k + 2][t + 1] <= hengline[k + 1][t]))
				{
					grid_index_6_1 = (float)(hengline[k + 2][t + 1] - hengline[k][t]) / (hengline[k + 2][t + 1] - hengline[k + 1][t + 1]);
					grid_index_6_2 = 1 - grid_index_6_1;
				}
				else if (hengline[k + 1][t + 1] <= hengline[k][t] && hengline[k + 2][t + 1] > hengline[k + 1][t])
				{
					grid_index_6_1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 2][t + 1] - hengline[k + 1][t + 1]);
					grid_index_6_2 = 1 - grid_index_6_1;
				}
				else if (hengline[k + 1][t + 1] > hengline[k][t] && hengline[k + 2][t + 1] <= hengline[k + 1][t])
				{
					grid_index_6_1 = 1.;
					grid_index_6_2 = 0.;
				}
				else if ((hengline[k + 1][t + 1] > hengline[k][t] && hengline[k + 1][t + 1] <= hengline[k + 1][t]) && hengline[k + 2][t + 1] >= hengline[k + 1][t])
				{
					grid_index_6_1 = (float)(hengline[k + 1][t] - hengline[k + 1][t + 1]) / (hengline[k + 2][t + 1] - hengline[k + 1][t + 1]);
					grid_index_6_2 = 1 - grid_index_6_1;
				}
				else
				{
					grid_index_6_1 = 0.;
					grid_index_6_2 = 1.;
				}
			}
			//对偶犹豫模糊熵
			float E_c1_1 = 0.;//w1隶属度
			float E_c1_2 = 0.;//w1非隶属度
			float E_c1_3 = 0.;//w1犹豫度
			float E_c2_1 = 0.;//w2隶属度
			float E_c2_2 = 0.;//w2非隶属度
			float E_c2_3 = 0.;//w2犹豫度
			float E_c3_1 = 0.;//w3隶属度
			float E_c3_2 = 0.;//w3非隶属度
			float E_c3_3 = 0.;//w3犹豫度
			float E1;//w1熵
			float E2;//w2熵
			float E3;//w3熵
			float w1;
			float w2;
			float w3;
			if (t == 0 && k == 0)//考虑右(5)、右下(6)网格
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_5_1 + pixel_index_6_1) / 2;
				E_c1_2 = (pixel_index_5_2 + pixel_index_6_2) / 2;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (stroke_index_5_1 + stroke_index_6_1) / 2;
				E_c2_2 = (stroke_index_5_2 + stroke_index_6_2) / 2;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (grid_index_5_1 + grid_index_6_1) / 2;
				E_c3_2 = (grid_index_5_2 + grid_index_6_2) / 2;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
				//计算相关系数
				c_c_5 = (w1 * pixel_index_5_1 + w2 * stroke_index_5_1 + w3 * grid_index_5_1) /
					(sqrt(w1 * (pow(pixel_index_5_1, 2) + pow(pixel_index_5_2, 2)) +
						w2 * (pow(stroke_index_5_1, 2) + pow(stroke_index_5_2, 2)) +
						w3 * (pow(grid_index_5_1, 2) + pow(grid_index_5_2, 2))));
				c_c_6 = (w1 * pixel_index_6_1 + w2 * stroke_index_6_1 + w3 * grid_index_6_1) /
					(sqrt(w1 * (pow(pixel_index_6_1, 2) + pow(pixel_index_6_2, 2)) +
						w2 * (pow(stroke_index_6_1, 2) + pow(stroke_index_6_2, 2)) +
						w3 * (pow(grid_index_6_1, 2) + pow(grid_index_6_2, 2))));
				//计算特征
				kuai = t * 5 + k;
				feature[kuai] = (temp_feature[kuai] + c_c_5 * (temp_feature[kuai + 5]) + c_c_6 * (temp_feature[kuai + 6])) * 1000 / SumH;
			}
			else if (t == 0 && k != 0 && k != 4)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 3;
				E_c1_2 = (pixel_index_4_2 + pixel_index_5_2 + pixel_index_6_2) / 3;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (stroke_index_4_1 + stroke_index_5_1 + stroke_index_6_1) / 3;
				E_c2_2 = (stroke_index_4_2 + stroke_index_5_2 + stroke_index_6_2) / 3;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (grid_index_4_1 + grid_index_5_1 + grid_index_6_1) / 3;
				E_c3_2 = (grid_index_4_2 + grid_index_5_2 + grid_index_6_2) / 3;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
				//计算相关系数
				c_c_4 = (w1 * pixel_index_4_1 + w2 * stroke_index_4_1 + w3 * grid_index_4_1) /
					(sqrt(w1 * (pow(pixel_index_4_1, 2) + pow(pixel_index_4_2, 2)) +
						w2 * (pow(stroke_index_4_1, 2) + pow(stroke_index_4_2, 2)) +
						w3 * (pow(grid_index_4_1, 2) + pow(grid_index_4_2, 2))));
				c_c_5 = (w1 * pixel_index_5_1 + w2 * stroke_index_5_1 + w3 * grid_index_5_1) /
					(sqrt(w1 * (pow(pixel_index_5_1, 2) + pow(pixel_index_5_2, 2)) +
						w2 * (pow(stroke_index_5_1, 2) + pow(stroke_index_5_2, 2)) +
						w3 * (pow(grid_index_5_1, 2) + pow(grid_index_5_2, 2))));
				c_c_6 = (w1 * pixel_index_6_1 + w2 * stroke_index_6_1 + w3 * grid_index_6_1) /
					(sqrt(w1 * (pow(pixel_index_6_1, 2) + pow(pixel_index_6_2, 2)) +
						w2 * (pow(stroke_index_6_1, 2) + pow(stroke_index_6_2, 2)) +
						w3 * (pow(grid_index_6_1, 2) + pow(grid_index_6_2, 2))));
				//计算特征
				kuai = t * 5 + k;
				feature[kuai] = (temp_feature[kuai] + c_c_4 * (temp_feature[kuai + 4]) + c_c_5 * (temp_feature[kuai + 5]) + c_c_6 * (temp_feature[kuai + 6])) * 1000 / SumH;
			}
			else if (t == 0 && k == 4)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_4_1 + pixel_index_5_1) / 2;
				E_c1_2 = (pixel_index_4_2 + pixel_index_5_2) / 2;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (stroke_index_4_1 + stroke_index_5_1) / 2;
				E_c2_2 = (stroke_index_4_2 + stroke_index_5_2) / 2;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (grid_index_4_1 + grid_index_5_1) / 2;
				E_c3_2 = (grid_index_4_2 + grid_index_5_2) / 2;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
				//计算相关系数
				c_c_4 = (w1 * pixel_index_4_1 + w2 * stroke_index_4_1 + w3 * grid_index_4_1) /
					(sqrt(w1 * (pow(pixel_index_4_1, 2) + pow(pixel_index_4_2, 2)) +
						w2 * (pow(stroke_index_4_1, 2) + pow(stroke_index_4_2, 2)) +
						w3 * (pow(grid_index_4_1, 2) + pow(grid_index_4_2, 2))));
				c_c_5 = (w1 * pixel_index_5_1 + w2 * stroke_index_5_1 + w3 * grid_index_5_1) /
					(sqrt(w1 * (pow(pixel_index_5_1, 2) + pow(pixel_index_5_2, 2)) +
						w2 * (pow(stroke_index_5_1, 2) + pow(stroke_index_5_2, 2)) +
						w3 * (pow(grid_index_5_1, 2) + pow(grid_index_5_2, 2))));
				//计算特征
				kuai = t * 5 + k;
				feature[kuai] = (temp_feature[kuai] + c_c_4 * (temp_feature[kuai + 4]) + c_c_5 * (temp_feature[kuai + 5])) * 1000 / SumH;
			}
			else if (t == 4 && k == 0)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_2_1 + pixel_index_3_1) / 2;
				E_c1_2 = (pixel_index_2_2 + pixel_index_3_2) / 2;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (stroke_index_2_1 + stroke_index_3_1) / 2;
				E_c2_2 = (stroke_index_2_2 + stroke_index_3_2) / 2;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (grid_index_2_1 + grid_index_3_1) / 2;
				E_c3_2 = (grid_index_2_2 + grid_index_3_2) / 2;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
				//计算相关系数
				c_c_2 = (w1 * pixel_index_2_1 + w2 * stroke_index_2_1 + w3 * grid_index_2_1) /
					(sqrt(w1 * (pow(pixel_index_2_1, 2) + pow(pixel_index_2_2, 2)) +
						w2 * (pow(stroke_index_2_1, 2) + pow(stroke_index_2_2, 2)) +
						w3 * (pow(grid_index_2_1, 2) + pow(grid_index_2_2, 2))));
				c_c_3 = (w1 * pixel_index_3_1 + w2 * stroke_index_3_1 + w3 * grid_index_3_1) /
					(sqrt(w1 * (pow(pixel_index_3_1, 2) + pow(pixel_index_3_2, 2)) +
						w2 * (pow(stroke_index_3_1, 2) + pow(stroke_index_3_2, 2)) +
						w3 * (pow(grid_index_3_1, 2) + pow(grid_index_3_2, 2))));
				//计算特征
				kuai = t * 5 + k;
				feature[kuai] = (temp_feature[kuai] + c_c_2 * (temp_feature[kuai + 4]) + c_c_3 * (temp_feature[kuai + 5])) * 1000 / SumH;
			}
			else if (t == 4 && k != 0 && k != 4)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
			}
			else if (t == 4 && k == 4)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
			}
			else if (t != 0 && t != 4 && k == 0)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
			}
			else if (t != 0 && t != 4 && k == 4)
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
			}
			else
			{
				//求像素距离指标的熵
				E_c1_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c1_3 = 1 - E_c1_1 - E_c1_2;
				if (E_c1_3 < 0)
				{
					E_c1_3 = 0.;
				}
				E1 = (2 * E_c1_1 * E_c1_2 + E_c1_3) / (pow(E_c1_1, 2) + pow(E_c1_2, 2) + E_c1_3);
				//求笔画位置指标的熵
				E_c2_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c2_3 = 1 - E_c2_1 - E_c2_2;
				if (E_c2_3 < 0)
				{
					E_c2_3 = 0.;
				}
				E2 = (2 * E_c2_1 * E_c2_2 + E_c2_3) / (pow(E_c2_1, 2) + pow(E_c2_2, 2) + E_c2_3);
				//求网格位置指标的熵
				E_c3_1 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_2 = (pixel_index_1_1 + pixel_index_2_1 + pixel_index_3_1 + pixel_index_4_1 + pixel_index_5_1 + pixel_index_6_1) / 6;
				E_c3_3 = 1 - E_c3_1 - E_c3_2;
				if (E_c3_3 < 0)
				{
					E_c3_3 = 0.;
				}
				E3 = (2 * E_c3_1 * E_c3_2 + E_c3_3) / (pow(E_c3_1, 2) + pow(E_c3_2, 2) + E_c3_3);
				//计算各属性权重
				w1 = (1 - E1) / (3 - E1 - E2 - E3);
				w2 = (1 - E2) / (3 - E1 - E2 - E3);
				w3 = (1 - E3) / (3 - E1 - E2 - E3);
			}

			for (j = ShuLine[t]; j < ShuLine[t + 1]; j++)
			{
				for (i = hengline[k][t]; i < hengline[k + 1][t]; i++)
				{
					kuai = t * 5 + k;
					feature[kuai] += H[i][j];
				}
			}
			if (SumH != 0)
				feature[kuai] = feature[kuai] * 1000 / SumH;
		}
	}
	//像素距离指标
	att_a1 = 0.;
	att_a2 = 0.;
	int dan = 0;
	for (t = 0; t < 4; t++)
	{
		for (k = 0; k < 5; k++)
		{
			for (j = ShuLine[t]; j < ShuLine[t + 1]; j++)
			{
				for (i = hengline[k][t]; i < hengline[k + 1][t]; i++)
				{
					if (H[i][j] == 1)
					{
						float temp_da1 = exp(-(pow(3 * (ShuLine[t + 1] - j) / (ShuLine[t + 1] - ShuLine[t]), 2) / 2));
						float temp_da2 = exp(-(pow(3 * (j - ShuLine[t]) / (ShuLine[t + 1] - ShuLine[t]), 2) / 2));
						att_a1 += temp_da1;
						att_a2 += temp_da2;
						dan += 1;
					}
				}
			}
		}
	}
	att_a1 = att_a1 / dan;
	att_a2 = att_a2 / dan;
	//笔画位置指标
	float **temp_H = new float*[m_imgHeight];
	for (i = 0; i < m_imgHeight; i++)
	{
		temp_H[i] = new float[m_imgWidth];
		for (j = 0; j < m_imgWidth; j++)
		{
			temp_H[i][j] = H[i][j];
		}
	}
	dan = 0;
	att_b1 = 0.;
	att_b2 = 0.;
	int bn1 = 0;
	int bn2 = 0;
	for (t = 1; t < 5; t++)
	{
		for (k = 0; k < 5; k++)
		{
			for (j = ShuLine[t]; j > ShuLine[t - 1]; j--)
			{
				for (i = hengline[k][t - 1]; i < hengline[k + 1][t - 1]; i++)
				{
					if (j == ShuLine[t])
					{
						for (int a = hengline[k][t - 1]; a < hengline[k + 1][t - 1]; a++)
						{
							if (temp_H[a][j] == 1)
							{
								bn1 += 1;
								temp_H[a][j] = 0;
								int temp_j1 = j - 1;
								while (temp_H[a][temp_j1] == 1)
								{
									temp_H[a][temp_j1] = 0;
									temp_j1--;
								}
								int temp_j2 = j + 1;
								while (temp_H[a][temp_j2] == 1)
								{
									temp_H[a][temp_j2] = 0;
									temp_j2++;
								}
							}
						}
					}
					else
					{
						if (temp_H[i][j] == 1)
						{
							bn2 += 1;
							continue;
						}
					}
				}
			}
			if (bn1 + bn2 != 0)
			{
				att_b1 += (float)bn1 / (bn1 + bn2);
				att_b2 += (float)bn2 / (bn1 + bn2);
				dan++;
			}
			else
			{
				att_b1 += 0;
				att_b2 += 0;
				dan++;
			}
		}
	}
	att_b1 = att_b1 / dan;
	att_b2 = att_b2 / dan;
	//网格位置指标
	att_c1 = 0.;
	att_c2 = 0.;
	dan = 0;
	for (t = 0; t < 4; t++)
	{
		for (k = 1; k < 4; k++)
		{
			for (j = ShuLine[t]; j < ShuLine[t + 1]; j++)
			{
				for (i = hengline[k][t]; i < hengline[k + 1][t]; i++)
				{
					if (hengline[k + 1][t] <= hengline[k][t + 1])
					{
						float temp_c1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k][t + 1] - hengline[k - 1][t + 1]);
						float temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						dan += 1;
					}
					if (hengline[k][t] <= hengline[k][t + 1] && hengline[k + 1][t] > hengline[k][t + 1] && hengline[k + 1][t] <= hengline[k + 1][t + 1])
					{
						float temp_c1 = (float)(hengline[k][t + 1] - hengline[k][t]) / (hengline[k][t + 1] - hengline[k - 1][t + 1]);
						float temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						temp_c1 = (float)(hengline[k + 1][t] - hengline[k][t + 1]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
						temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						dan += 2;
					}
					if (hengline[k][t] > hengline[k][t + 1] && hengline[k + 1][t] <= hengline[k + 1][t + 1])
					{
						float temp_c1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
						float temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						dan += 1;
					}
					if (hengline[k][t] <= hengline[k + 1][t + 1] && hengline[k + 1][t] > hengline[k + 1][t + 1] && hengline[k + 1][t] <= hengline[k + 2][t + 1])
					{
						float temp_c1 = (float)(hengline[k + 1][t + 1] - hengline[k][t]) / (hengline[k + 1][t + 1] - hengline[k][t + 1]);
						float temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						temp_c1 = (float)(hengline[k + 1][t] - hengline[k + 1][t + 1]) / (hengline[k + 2][t + 1] - hengline[k + 1][t + 1]);
						temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						dan += 2;
					}
					if (hengline[k][t] > hengline[k + 1][t + 1] && hengline[k + 1][t] <= hengline[k + 1][t + 1])
					{
						float temp_c1 = (float)(hengline[k + 1][t] - hengline[k][t]) / (hengline[k + 2][t + 1] - hengline[k + 1][t + 1]);
						float temp_c2 = 1 - temp_c1;
						att_c1 += temp_c1;
						att_c2 += temp_c2;
						dan += 1;
					}
				}
			}
		}
	}
	att_c1 = att_c1 / dan;
	att_c2 = att_c2 / dan;



	//竖笔画特征
	for (k = 0; k < 5; k++)
	{
		for (t = 0; t < 5; t++)
		{
			for (i = HengLine[k]; i < HengLine[k + 1]; i++)
				for (j = shuline[k][t]; j < shuline[k][t + 1]; j++)
				{
					kuai = k * 5 + t;
					feature[25 + kuai] += S[i][j];
				}
			if (SumS != 0)
				feature[25 + kuai] = feature[25 + kuai] * 1000 / SumS;
		}
	}
	//撇笔画特征
	for (t = 0; t < 5; t++)
	{
		for (k = 0; k < 5; k++)
		{
			for (j = ShuLine1[t]; j < ShuLine1[t + 1]; j++)
			{
				for (i = hengline1[k][t]; i < hengline1[k + 1][t]; i++)
				{
					kuai = t * 5 + k;
					feature[50 + kuai] += P[i][j];
				}
			}
			if (SumP != 0)
				feature[50 + kuai] = feature[50 + kuai] * 1000 / SumP;
		}
	}
	//捺笔画特征
	for (k = 0; k < 5; k++)
	{
		for (t = 0; t < 5; t++)
		{
			for (i = HengLine1[k]; i < HengLine1[k + 1]; i++)
				for (j = shuline1[k][t]; j < shuline1[k][t + 1]; j++)
				{
					kuai = k * 5 + t;
					feature[75 + kuai] += N[i][j];
				}
			if (SumN != 0)
				feature[75 + kuai] = feature[75 + kuai] * 1000 / SumN;
		}
	}
	//数据库操作
	//Database database;
	//database.ConnectDB();
	//database.InsertFeature(filename, feature,"DHFS");
	//database.CloseConnect();

	for (i = 0; i < m_imgHeight; i++)
	{
		delete[] H[i];
		delete[] S[i];
	}
	delete[] H;
	delete[] S;
	for (i = 0; i < m_imgHeight + m_imgWidth + 1; i++)
	{
		delete[] P[i];
		delete[] N[i];
	}
	delete[] P;
	delete[] N;
}