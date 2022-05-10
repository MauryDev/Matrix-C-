#pragma once
#include <initializer_list>
#include <vector>
struct MatrixAddress
{
	int L;
	int C;
	inline MatrixAddress operator =(std::initializer_list<int> s)
	{
		MatrixAddress e;
		e.L = *s.begin();
		e.C = *(s.begin() + 1);

		return e;
	}
	inline MatrixAddress operator =(std::initializer_list<size_t> s)
	{
		MatrixAddress e;
		e.L = *s.begin();
		e.C = *(s.begin() + 1);

		return e;
	}
	inline static MatrixAddress FromAddressH(int line, int column)
	{
		MatrixAddress address;
		address.L = line-1;
		address.C = column-1;
		return address;
	}
};

template<typename T>
class Matrix
{
public:
	typedef T(*MatrixCallback) (int L, int C);
	inline Matrix(int l,int c)
	{
		this->L = l;
		this->C = c;
		buffer = new T*[L];
		for (int i = 0; i < L; i++)
		{
			buffer[i] = new T[C];
		}
	}
	inline Matrix(std::initializer_list<std::initializer_list<T>> vals)
	{
		auto l = vals.size();
		std::initializer_list<T> first = *(vals.begin());
		auto c = (l > 0) ? first.size() : 0;
		Init(l, c, vals);
	}
	inline Matrix(int l, int c,std::initializer_list<std::initializer_list<T>> vals)
	{
		Init(l, c, vals);
	}
	inline void BuildMatrix(MatrixCallback callback)
	{
		for (size_t l = 0; l < L; l++)
		{
			for (size_t c = 0; c < C; c++)
			{
				this->buffer[l][c] = callback(l+1, c+1);
			}
		}
	}
	inline T& operator[](MatrixAddress address)
	{
		auto line = address.L;
		auto column = address.C;
		if (line >= 0 && line < L && column >= 0 && column < C)
		{
			return buffer[line][column];
		}
		else
		{
			throw "Linha ou column fora do intervalo";
		}
	}
	inline bool IsMatrixLine()
	{
		return L == 1;
	}
	inline bool IsMatrixColumn()
	{
		return C == 1;
	}
	inline bool IsMatrixNull()
	{
		bool result = false;
		for (size_t c = 0; c < C; c++)
		{
			T* column = buffer[c];
			for (size_t l = 0; l < L; l++)
			{
				T line = column[c];

				if (line != 0)
				{
					return false;
				}
				else
				{
					result = true;
				}
			}
		}
		return result;
	}
	inline bool IsMatrixSquare()
	{
		return L == C;
	}
	inline bool IsMatrixTransposta(Matrix<T> otherMatrix)
	{
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				auto val1 = (*this)[{l, c}];

				auto val2 = otherMatrix[{c, l}];
				if (val1 != val2)
				{
					return false;
				}
			}
		}
		return true;
	}
	inline Matrix<T> ToMatrixTransposta()
	{
		Matrix<T> ret(C,L);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T& val2 = ret[{c, l}];
				val2 = val1;
			}
		}
		return ret;
	}

	inline Matrix<T> ToMatrixOpposite()
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T& val2 = ret[{l, c}];
				val2 = -val1;
			}
		}
		return ret;
	}
	inline bool IsMatrixOpposite(Matrix<T> other)
	{
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T& val2 = other[{l, c}];
				if (val2 != -val1)
				{
					return false;
				}
			}
		}
		return true;
	}
	inline bool IsMatrixIdentity()
	{
		if (!IsMatrixSquare())
		{
			return false;
		}
		bool result = false;
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];

				if (l == c)
				{
					if (val1 != 1)
					{
						return false;
					}
					else
					{
						result = true;
					}
				}
				else
				{
					if (val1 != 0)
					{
						return false;
					}
					else
					{
						result = true;
					}
				}
				
			}
		}
		return result;
	}
	inline bool IsMatrixEqualDimension(Matrix<T> other)
	{
		return other.C == this->C && other.L == this->L;
	}
	inline bool IsMatrixEqual(Matrix<T> other)
	{
		if (!IsMatrixEqualDimension(other))
		{
			return false;
		}
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T val2 = other[{l, c}];
				if (val1 != val2)
				{
					return false;
				}
			}
		}
		return true;
	}
	inline Matrix<T> operator +(Matrix<T> other)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T val2 = other[{l, c}];
				ret[{l, c}] = val1 + val2;
			}
		}
		return ret;
	}
	inline Matrix<T> operator +(T value)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				ret[{l, c}] = val1 + value;
			}
		}
		return ret;
	}
	inline Matrix<T> operator -(Matrix<T> other)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				T val2 = other[{l, c}];
				ret[{l, c}] = val1 - val2;
			}
		}
		return ret;
	}
	inline Matrix<T> operator -(T other)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				T val1 = (*this)[{l, c}];
				ret[{l, c}] = val1 - other;
			}
		}
		return ret;
	}
	inline Matrix<T> operator *(Matrix<T> other)
	{
		auto C2 = other.C;
		auto L2 = other.C;

		if (!(L == C2 || L2 == C) )
		{
			throw "Invalid Colunm";
		}
		Matrix<T> ret(L, C2);
		for (size_t l = 0; l < L; l++)
		{
			for (size_t c = 0; c < C2; c++)
			{
				T clc = 0;
				for (size_t i = 0; i < C; i++)
				{
					clc += (*this)[{(int)l, (int)i}] * other[{(int)i, (int)c}];
				}
				ret[{(int)l, (int)c}] = clc;
			}
		}
		return ret;
	}
	inline Matrix<T> operator *(T other)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{				
				ret[{l, c}] = (*this)[{l, c}] * other;
			}
		}
		return ret;
	}
	inline Matrix<T> Pow(T other)
	{
		Matrix<T> ret(L, C);
		for (size_t c = 0; c < C; c++)
		{
			for (size_t l = 0; l < L; l++)
			{
				ret[{l, c}] = pow((*this)[{l, c}], other);
			}
		}
		return ret;
	}
	inline std::vector<T> DiagonalPrimary()
	{
		if (IsMatrixSquare())
		{
			std::vector<T> ret(this->L);

			for (size_t i = 0; i < L; i++)
			{
				ret[i] = (*this)[{i, i}];
			}
			return ret;
		}
		return std::vector<T>();
	}
	inline std::vector<T> DiagonalSecondary()
	{
		if (IsMatrixSquare())
		{
			std::vector<T> ret(this->L);

			for (size_t i = 0; i < L; i++)
			{
				ret[i] = (*this)[{i, L - i - 1}];
			}
			return ret;

		}
		return std::vector<T>();
	}
	inline friend std::ostream& operator <<(std::ostream& os_stream, Matrix<T> Matrix)
	{
		for (size_t l = 0; l < Matrix.L; l++)
		{
			os_stream << "| ";
			for (size_t c = 0; c < Matrix.C; c++)
			{
				os_stream << Matrix[{(int)l, (int)c}] << " ";
			}
			os_stream << "|\n";

		}
		return os_stream;
	}

	inline void Free()
	{
		for (size_t l = 0; l < L; l++)
		{
			delete buffer[l];
		}
		
		delete buffer;
		buffer = nullptr;
	}
	inline T Determinate()
	{
		auto that = (*this);
		if (IsMatrixSquare())
		{
			if (C == 1)
			{
				return buffer[0][0];
			}
			else if (C == 2)
			{
				return buffer[0][0] * buffer[1][1] - buffer[0][1] * buffer[1][0];
			}
			else
			{
				T det = T();
				Matrix<T> matrixtemp(L-1,C-1);

				for (size_t c1 = 0; c1 < C; c1++)
				{
					for (size_t l = 1; l < L; l++)
					{
						size_t c3 = 0;
						for (size_t c = 0; c < C; c++)
						{
							if (c1 != c)
							{
								matrixtemp[{(int)l - 1, (int)c3}] = that[{(int)l, (int)c}];
								c3++;
							}
						}
					}
					det = det + that[{0, (int)c1}] * pow(-1, (int)c1) * matrixtemp.Determinate();
				}
				matrixtemp.Free();
				return det;
			}
			
		}
		else
		{
			return T();
		}
	}
	inline Matrix<T> Inverse()
	{
		Matrix<T> ret(L, C);
		T determinate = Determinate();
		if (C == 2)
		{
			for (size_t c = 0; c < C; c++)
			{
				for (size_t l = 0; l < L; l++)
				{
					if (c == l)
					{
						ret.buffer[L - 1 - l][C - 1 - c] = buffer[l][c];
					}
					else
					{
						ret.buffer[l][c] = -buffer[l][c];
					}
				}
			}
			for (size_t c = 0; c < C; c++)
			{
				for (size_t l = 0; l < L; l++)
				{
					ret.buffer[l][c] = (1.0 / determinate) * ret.buffer[l][c];

				}
			}
		}
		else
		{
			Matrix<T> cofator = Cofactor();
			for (int c = 0; c < C; c++)
			{
				for (int l = 0; l < L; l++)
				{
					
					ret[{(int)c, (int)l}] = cofator[{(int)l,(int)c}] / determinate;
				}
			}
			cofator.Free();
		}
		
		return ret;
	}
	inline Matrix<T> Cofactor()
	{
		Matrix<T> matrixtemp(L - 1, C - 1);
		Matrix<T> ret(L, C );
		auto that = *this;
		for (size_t l1 = 0; l1 < L; l1++)
		{
			for (size_t c1 = 0; c1 < C; c1++)
			{
				size_t c3 = 0;
				for (size_t c = 0; c < C; c++)
				{

					if (c == c1)
					{
						continue;
					}
					size_t l3 = 0;
					for (size_t l = 0; l < L; l++)
					{
					
					

						if (l != l1)
						{
							matrixtemp[{(int)l3, (int)c3}] = that[{(int)l, (int)c}];
							l3++;
						}
					}
					c3++;

				}
				ret[{(int)l1, (int)c1}] = pow(-1, c1 + l1 + 2 ) * matrixtemp.Determinate();
			}
		}
		matrixtemp.Free();
		return ret;
	}

private:
	inline void Init(int l, int c, std::initializer_list<std::initializer_list<T>> vals)
	{
		this->L = l;
		this->C = c;
		if (IsValid(vals))
		{
			buffer = new T*[L];
			for (size_t l = 0; l < L; l++)
			{
				buffer[l] = new T[C];
				std::initializer_list<T> current = *(vals.begin() + l);
				auto linedeagora = buffer[l];
				auto lenC = current.size();
				for (size_t c = 0; c < C && c < lenC; c++)
				{
					// column
					T &columndeagora = linedeagora[c];
					columndeagora = *(current.begin() + c);
				}
			}
		}
		else
		{
			throw "Tamanhos errados";
		}
	}
	inline bool IsValid(std::initializer_list<std::initializer_list<T>> vals)
	{
		if (C < 0 || L < 0)
		{
			return false;
		}
		auto line = vals.size();
		if (line != L)
		{
			return false;
		}
		for (std::initializer_list<T> lines : vals)
		{
			auto columns = lines.size();
			if ((int)columns != C)
			{
				return false;
			}
		}
		return true;
	}

	T** buffer;
	int L, C;
};

