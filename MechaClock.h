#ifndef _mecha_clock_
#define _mecha_clock_
#define INTERVAL 0
//:set tabstop=8

constexpr uint8_t DIGITS = 4;
constexpr uint8_t SEGMENTS = 7;
// Массивы калибровки, если не спокойно на душе - можно сделать const,
// но их необходимо менять в скетче калибровки...
extern uint8_t ON[DIGITS][SEGMENTS];
extern uint8_t OFF[DIGITS][SEGMENTS];

#ifdef INTERVAL
uint8_t interval = INTERVAL;
#endif

// Дефайны скопированы из библиотеки iarduino_I2C_4LED
// DEC 10  // print(0xFFF , DEC); вывод числа в 10-тичной системе = 4095
#define LEN1 11 // print(1234, LEN1); вывод числа в 1 разряде = •••4
#define LEN2 12 // print(123, LEN2); вывод числа в 2 разрядах = ••23
#define LEN3 13 // print(12, LEN3); вывод числа в 3 разрядах = •012
#define LEN4 14 // print(1, LEN4); вывод числа в 4 разрядах = 0001
// HEX 16  // print(4095, HEX); вывод числа в 16-ричной системе = •FFF
#define TIME 17  // print(1, 2, TIME); вывод времени = 01:02
#define TEMP 18  // print(12, TEMP); вывод температуры = •12°
#define LEFT 19  // print(12, [POS4], LEFT); вывод числа со сдвигом от влево  от 4 позиции = ••12
#define RIGHT 20 // print(12, [POS1], RIGHT); вывод числа со сдвигом от вправо от 1 позиции = 12••
#define POS1 21  // print(12, POS1 ,[LEFT]); вывод числа со сдвигом от влево  от 1 позиции = 2•••
#define POS2 22  // print(12, POS2 ,[LEFT]); вывод числа со сдвигом от влево  от 2 позиции = 12••
#define POS3 23  // print(12, POS3 ,[LEFT]); вывод числа со сдвигом от влево  от 3 позиции = •12•
#define POS4 24  // print(12, POS4 ,[LEFT]); вывод числа со сдвигом от влево  от 4 позиции = ••12

// Карта подключения серво к двум экспандерам.
// Сегменты:                          G  F  E  D  C  B  A
// Индексы выводов экспандера
constexpr uint8_t _exp_io[SEGMENTS] { 3, 0, 2, 1, 0, 2, 1 };
// Чётный/нечётный экспандер (1 - это чётный, C'est la vie)
constexpr uint8_t _exp_num[SEGMENTS] { 0, 0, 1, 1, 1, 0, 0 };

// Буфер символов
char data[DIGITS];

// Строка для вывода, скопировано из библиотеки iarduino_I2C_4LED
// для совместимости с функциями print
char arrString[11];

// Переменная для совместимости с функциями print
const uint8_t valAddr = 1;
// Скопировано из библиотеки iarduino_I2C_4LED
char symZero;
uint8_t valPos, varLen, valDir, valType, valPart;

// Класс одной цифры.
class Digit {
	public:
		Digit(){};
		Digit(iarduino_I2C_Expander* E, uint8_t d):
			_E(E + 2 * d),
			_d(d)
		{
		}

		// Установка цифры
		void set(const char& dig)
		{
			_set(_encode(dig));
		}

		// Отставить управлять сервоприводами
		void release()
		{
			_release();
		}

	private:

		void _set(const uint8_t& ch)
		{
			if (_d == 255)
				return;

			for (uint8_t i = 0; i < SEGMENTS; i++) {
				if ((ch >> (i+1)) & 1) {
					// например: экспандер[чётный/нечётный].servoWrite(вывод, угол);
					// чётный/нечётный и вывод берутся из таблиц выше
					_E[_exp_num[i]].servoWrite(_exp_io[i], ON[_d][i]);
				}
				else {
					_E[_exp_num[i]].servoWrite(_exp_io[i], OFF[_d][i]);
				}
#ifdef INTERVAL
				delay(interval);
#endif
			}
		}

		void _release()
		{
			if (_d == 255)
				return;

			for (uint8_t i = 0; i < SEGMENTS; i++)
				_E[_exp_num[i]].pinMode(_exp_io[i], INPUT);
		}

		// Скопировано из библиотеки iarduino_I2C_4LED
		uint8_t _encode(const char& sym)
		{
			switch (sym) {
				case 'o':
				case 'O':
				case '0':
					return 0xFC;
				case 'i':
				case 'I':
				case '1':
					return 0x60;
				case '2':
					return 0xDA;
				case '3':
					return 0xF2;
				case '4':
					return 0x66;
				case 's':
				case 'S':
				case '5':
					return 0xB6;
				case '6':
					return 0xBE;
				case '7':
					return 0xE0;
				case '8':
					return 0xFE;
				case '9':
					return 0xF6;
				case 'a':
				case 'A':
					return 0xEE;
				case 'b':
				case 'B':
					return 0x3E;
				case 'c':
				case 'C':
					return 0x9C;
				case 'd':
				case 'D':
					return 0x7A;
				case 'e':
				case 'E':
					return 0x9E;
				case 'f':
				case 'F':
					return 0x8E;
				case 'g':
				case 'G':
					return 0xBC;
				case 'h':
				case 'H':
					return 0x9E;
				case 'j':
				case 'J':
					return 0x78;
				case 'l':
				case 'L':
					return 0x1C;
				case 'n':
				case 'N':
					return 0x2A;
				case 'p':
				case 'P':
					return 0xCE;
				case 't':
				case 'T':
					return 0x1E;
				case 'u':
				case 'U':
					return 0x7C;
				case '-':
					return 0x02;
				case '_':
					return 0x10;
				case '*':
					return 0xC6;
				case '"':
				case '\'':
					return 0x44;
				default:
					return 0x00;
			}
		}

		// Указатель на массив расширителей выводов
		iarduino_I2C_Expander* _E;
		uint8_t _d = 255;
};

// Класс часов
class MechaClock {
	public:
		// Создаём объект, инициализируем поле
	       	// указателя на массив объектов расширителей выводов
		MechaClock(iarduino_I2C_Expander* E, uint8_t i = INTERVAL, uint8_t dig_num = DIGITS):
				_E(E)
		{
#ifdef INTERVAL
			// Устанавливаем интервал переключения сегментов
			if (i != INTERVAL)
				interval = i;
#endif
			if (dig_num > 4)
				dig_num = 4;

			_dig_num = dig_num;

			// Заполняем поля объектов цифр
			for (size_t i = 0; i < _dig_num; i++)
				_dig[i] = Digit(E, i);
		}

		// Отсавить управлять всеми сервоприводами
		void releaseAll() {
			_releaseAll();
		}

		// следующие функции скопированны из библиотеки iarduino_I2C_4LED
		void print(    int16_t i,          int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle('0',j1,j2,j3,j4,j5); if(valType==TIME){_printNumT(i,j1);}else{_printNumI(i);}}
		void print(    int16_t i, char j0, int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(    int16_t i,          int j1    , char j0, int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(    int16_t i,          int j1    ,          int j2    , char j0, int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(    int16_t i,          int j1    ,          int j2    ,          int j3    , char j0, int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(    int16_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    , char j0, int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(    int16_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    ,          int j5,    char j0){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(i);}
		void print(   uint16_t i,          int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle('0',j1,j2,j3,j4,j5); if(valType==TIME){_printNumT(int16_t(i),j1);}else{_printNumI(int16_t(i));}}
		void print(   uint16_t i, char j0, int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint16_t i,          int j1    , char j0, int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint16_t i,          int j1    ,          int j2    , char j0, int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint16_t i,          int j1    ,          int j2    ,          int j3    , char j0, int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint16_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    , char j0, int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint16_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    ,          int j5,    char j0){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle('0',j1,j2,j3,j4,j5); if(valType==TIME){_printNumT(int16_t(i),j1);}else{_printNumI(int16_t(i));}}
		void print(    int32_t i, char j0, int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1    , char j0, int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1    ,          int j2    , char j0, int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1    ,          int j2    ,          int j3    , char j0, int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    , char j0, int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(    int32_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    ,          int j5,    char j0){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle('0',j1,j2,j3,j4,j5); if(valType==TIME){_printNumT(int16_t(i),j1);}else{_printNumI(int16_t(i));}}
		void print(   uint32_t i, char j0, int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1    , char j0, int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1    ,          int j2    , char j0, int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1    ,          int j2    ,          int j3    , char j0, int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    , char j0, int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(   uint32_t i,          int j1    ,          int j2    ,          int j3    ,          int j4    ,          int j5,    char j0){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumI(int16_t(i));}
		void print(     double i,          int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle('0',j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i, char j0, int j1=255,          int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i,          int j1    , char j0, int j2=255,          int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i,          int j1    ,          int j2    , char j0, int j3=255,          int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i,          int j1    ,          int j2    ,          int j3    , char j0, int j4=255,          int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i,          int j1    ,          int j2    ,          int j3    ,          int j4    , char j0, int j5=255        ){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(     double i,          int j1    ,          int j2    ,          int j3    ,          int j4    ,          int j5,    char j0){_sortStyle( j0,j1,j2,j3,j4,j5); _printNumF(i);}
		void print(      char *i){_sortStyle(' ',255,255,255,255,255);                                                                                                       _printNumS(i);}
		void print(const char *i){_sortStyle(' ',255,255,255,255,255); char j[10]; j[9]=0; for(uint8_t k=0; k<9; k++){j[k]=i[k]; if(i[k]==0){k=10;}}                         _printNumS(j);}
		void print(     String i){_sortStyle(' ',255,255,255,255,255); char j[10]; j[9]=0; for(uint8_t k=0; k<9; k++){j[k]=i[k]; if(i[k]==0){k=10;}}                         _printNumS(j);}
		void print(      int i[]){_sortStyle(' ',255,255,255,255,255);                                                                                                       _printNumA(i);}


	private:

		void _writeData()
		{
			data[0] = arrString[0];
			data[1] = arrString[2];
			data[2] = arrString[5];
			data[3] = arrString[7];
			for (size_t i = 0; i < DIGITS; i++)
				_dig[i].set(data[i]);
		}

		void _writeData(const uint8_t dig, const uint8_t pos)
		{
			if (pos > DIGITS)
				return;

			_dig[pos].set(dig);
		}

		//  Параметры:   символ, параметр1, параметр2, параметр3, параметр4, параметр5.
		void _sortStyle(char j0, uint8_t j1, uint8_t j2, uint8_t j3, uint8_t j4, uint8_t j5)
		{
			if (valAddr) { //	Если модуль был инициализирован, то ...
				//	Собираем все полученные параметры в массив:																		//
				uint8_t j[5] = {
					j1,
					j2,
					j3,
					j4,
					j5
				}; // Определяем массив полученными параметрами.
				// Сбрасываем переменные для хранения параметров в значения по умолчанию:
				symZero = j0;  //' ' символ заменяющий знаки 0 перед числом (если «varLen» больше разрядности числа).
				varLen = 255;  // 0,1,2,3,4,255 длинна (количество рязрядов) выводимого числа, 255-по размеру числа.
				valPos = 255;  // POS1 - POS4 позиция (номер разряда) выводимого числа.
				valDir = LEFT;  // LEFT / RIGHT направление сдвига от указанной позиции.
				valType = DEC;  // DEC/HEX/TIME/TEMP тип выводимого числа.
				valPart = 255;  // 0,1,2,3,4,255 количество знаков после запятой, 255-не указано.
				arrString[0] = 0;  // "X.X.:X.X." строка символов для вывода на экран.
				// Находим тип выводимого числа «valType»:
				for (uint8_t i = 0; i < 5; i++) {
					if (j[i] == DEC || j[i] == HEX || j[i] == TIME || j[i] == TEMP) {
						valType = j[i];
					}
				}
				// Находим длину «varLen», позицию «valPos», направление «valDir» и количество знаков после запятой «valPart»:		//
				for (uint8_t i = valType == TIME ? 1 : 0; i < 5; i++) { //	Проходим по всем полученным параметрам ...
					if (j[i] == LEN1 || j[i] == LEN2 || j[i] == LEN3 || j[i] == LEN4) {
						varLen = j[i];
					} //	Определяем длину (количество рязрядов) «varLen» выводимого числа.
					if (j[i] == POS1 || j[i] == POS2 || j[i] == POS3 || j[i] == POS4) {
						valPos = j[i];
					} //	Определяем позицию (номер разряда) «valPos» выводимого числа.
					if (j[i] == LEFT || j[i] == RIGHT) {
						valDir = j[i];
					} //	Определяем направление сдвига «valDir» от позиции «valPos».
				}
				if (j1 < 5) {
					valPart = j1;
				} // Определяем количество знаков после запятой «valPart».
				if (valPos == 255) {
					valPos = valDir == LEFT ? POS4 : POS1;
				} // Определяем позицию (номер разряда) «valPos» выводимого числа если она не указана.
			}
		} // end _sortStyle

		// Вывод целого числа:
		void _printNumI(int16_t i)
		{
			if (valAddr) { // Если модуль был инициализирован, то ...
				// Перенапраяляем вывод на число с плавающей точкой:
				if (valPart > 0 && valPart != 255) {
					_printNumF(double(i));
					return;
				}
				// Создаём временные переменные:
				char l[10];
				l[0] = 0;
				uint8_t len = 0;
				bool neg = 0, fneg = 0; //
				// Сохраняем знак и получаем модуль числа i:
				if (i < 0) {
					neg = 1;
					fneg = 1;
					i *= -1;
				}
				// Преобразуем число i в строку l и определяем её длину len:
				itoa(i, l, (valType == HEX ? HEX : DEC));
				if (valType == TEMP) {
					strcat(l, "*");
				}
				len = strlen(l); //
				// Удаляем из начала строки l лишние символы, если таковые есть:
				if (varLen < 255 && (varLen - LEN1 + 1) < len) {
					_strDelete(l, (len - (varLen - LEN1 + 1)), 0);
				}
				len = strlen(l); //
				// Добавляем в начало строки l недостающие символы '0' и знак числа, если таковые необходимы:
				if (varLen < 255 && (varLen - LEN1 + 1) > len) {
					_strInsert(l, symZero, ((varLen - LEN1) - len), 0);
					_strInsert(l, (neg ? '-' : symZero), 1, 0);
					fneg = 0;
				}
				len = strlen(l);
				// Если в предыдущем условии знак не добавлен, то добавляем его сейчас:
				if (fneg) {
					if (varLen < 255 && (varLen - LEN1) < len) {
						_strDelete(l, 1, 0);
					}
					_strInsert(l, '-', 1, 0);
					fneg = 0;
				}
				len = strlen(l);
				// Сдвигаем строку l вправо на указанную в valPos позицию:
				if (valPos > POS1) {
					_strInsert(l, ' ', (valPos - POS1), 0);
				}
				// Сдвигаем строку влево если указано смещение valDir==LEFT:
				if (valDir == LEFT) {
					_strDelete(l, len - 1, 0);
				}
				len = strlen(l);
				// Срезаем символы за пределами индикатора:
				l[4] = 0;
				// Выводим строку:
				_printNumS(l);
			}
		} // end _printNumI

		// Вывод числа с плавающей точкой:
		void _printNumF(double i)
		{
			if (valAddr) { // Если модуль был инициализирован, то ...
				// Перенапраяляем вывод на число без запятой:
				if (valPart == 0) {
					_printNumI(int16_t(i));
					return;
				}
				if (valPart == 255) {
					valPart = 1;
				}
				if (valPart > 4) {
					valPart = 4;
				}
				// Создаём временные переменные:
				char l[10];
				l[0] = 0;
				uint8_t len = 0;
				bool neg = 0, fneg = 0, fneg1 = 0; //
				// Сохраняем знак и получаем модуль числа i:
				if (i < 0) {
					neg = 1;
					fneg = 1;
					fneg1 = 1;
					i *= -1;
				}
				// Разбиваем число на челое и дробное:
				int16_t i1 = int16_t(i);
				int16_t i2 = int32_t(i * 10000) % 10000;
				// Собираем целое i1 и дробное i2 в строку l и определяем её длину len:
				itoa(i1, l, (valType == HEX ? HEX : DEC));
				strcat(l, ".");
				len = strlen(l);
				if (i2 < 1000) {
					strcat(l, "0");
				}
				if (i2 < 100) {
					strcat(l, "0");
				}
				if (i2 < 10) {
					strcat(l, "0");
				}
				itoa(i2, & l[strlen(l)], (valType == HEX ? HEX : DEC));
				l[len + valPart] = 0;
				if (valType == TEMP) {
					strcat(l, "*");
				}
				len = strlen(l);
				// Удаляем из начала строки l лишние символы, если таковые есть:
				if (varLen < 255 && (varLen - LEN1 + 2) < len) {
					_strDelete(l, (len - (varLen - LEN1 + 2)), 0);
				}
				len = strlen(l); //
				// Добавляем в начало строки l недостающие символы '0' и знак числа, если таковые необходимы:
				if (varLen < 255 && (varLen - LEN1 + 2) > len) {
					_strInsert(l, symZero, ((varLen - LEN1 + 1) - len), 0);
					_strInsert(l, (neg ? '-' : symZero), 1, 0);
					fneg = 0;
				}
				len = strlen(l);
				// Если в предыдущем условии знак не добавлен, то добавляем его сейчас:
				if (fneg) {
					if (varLen < 255 && (varLen - LEN1 + 1) < len) {
						_strDelete(l, 1, 0);
					}
					_strInsert(l, '-', 1, 0);
					fneg = 0;
				}
				len = strlen(l);
				// Сдвигаем строку l вправо на указанную в valPos позицию:
				if (valPos > POS1) {
					_strInsert(l, ' ', (valPos - POS1), 0);
				}
				// Сдвигаем строку влево если указано смещение valDir==LEFT:
				if (valDir == LEFT) {
					_strDelete(l, len - 2, 0);
				}
				len = strlen(l); //
				// Проверка наличия запятых и знаков:
				if (l[0] == '.') {
					_strDelete(l, 1, 0);
				}
				fneg = 1;
				if (l[1] == '.') {
					fneg = 0;
				}
				if (l[2] == '.') {
					fneg = 0;
				}
				if (l[3] == '.') {
					fneg = 0;
				}
				if (l[4] == '.') {
					fneg = 0;
				}
				if (fneg) {
					_strDelete(l, 1, 0);
				}
				if (l[0] == '.') {
					_strDelete(l, 1, 0);
				}
				fneg = 1;
				if (l[0] == '-') {
					fneg = 0;
				}
				if (l[1] == '-') {
					fneg = 0;
				}
				if (l[2] == '-') {
					fneg = 0;
				}
				if (l[3] == '-') {
					fneg = 0;
				}
				if (l[4] == '-') {
					fneg = 0;
				}
				if (fneg && fneg1) {
					l[0] = '-';
				}
				fneg = 0;
				if (l[1] == '.') {
					fneg = 1;
				}
				if (l[2] == '.') {
					fneg = 1;
				}
				if (l[3] == '.') {
					fneg = 1;
				}
				if (l[4] == '.') {
					fneg = 1;
				}
				l[4 + fneg] = 0;
				// Выводим строку:
				_printNumS(l);
			}
		} // end _printNumF

		// Вывод времени:
		void _printNumT(uint8_t i, uint8_t j)
		{
			if (valAddr) { // Если модуль был инициализирован, то ...
				char l[10];
				l[0] = 0; //
				if (i > 59) {
					i = 59;
				}
				if (j > 59) {
					j = 59;
				} //
				if (i < 10) {
					strcat(l, "0");
				} //	Добавляем символ 0 перед часами, если таковой нужен.
				itoa(i, & l[strlen(l)], DEC); // Добавляем в строку часы.
				strcat(l, ":"); // Добавляем в строку разделитель.
				if (j < 10) {
					strcat(l, "0");
				} //	Добавляем символ 0 перед минутами, если таковой нужен.
				itoa(j, & l[strlen(l)], DEC); // Добавляем в строку часы.
				//	Выводим строку:
				_printNumS(l);
			}
		}

		// Вывод строки:
		void _printNumS(char * i)
		{
			if (valAddr) { //	Если модуль был инициализирован, то ...
				uint8_t f = 0;
				arrString[0] = 0; //	Чистим строку «arrString».
				for (uint8_t j = 0, k = 1; j < 10; j++, k++) { //	Проходим по всем доступным символам строки «i».
					if (strlen(i) > j) { //
						if (i[j] == ':' || i[j] == ';' || i[j] == '=') {
							f = k;
							k--;
						} else //
						{ //
							if (i[j] == '.' || i[j] == ',') {
								if (k % 2 == 1) {
									if (k < 10) {
										arrString[k - 1] = ' ';
									}
									k++;
								}
							} //	Если требуется вывести "." или ":".
							else {
								if (k % 2 == 0) {
									if (k < 10) {
										arrString[k - 1] = ' ';
									}
									k++;
								}
							} //	Если требуется вывести число.
							if (k < 10) {
								arrString[k - 1] = i[j];
							} //	Дополняем строку указанным символом.
						} //
					} else {
						if (k < 10) {
							arrString[k - 1] = ' ';
						}
					} //	Дополняем недостающие символы.
				}
				arrString[9] = 0; //
				if (f == 1) {
					arrString[5] = 0;
					_strInsert(arrString, ' ', 4, 0);
				} //
				if (f == 2 || f == 3) {
					arrString[7] = 0;
					_strInsert(arrString, ' ', 2, 0);
				} //
				if (f == 6 || f == 7) {
					_strDelete(arrString, 2, 0);
				} //
				if (f == 8 || f == 9) {
					_strDelete(arrString, 4, 0);
				} //
				_strInsert(arrString, (f > 0 ? ':' : ' '), 1, 4);
				for (uint8_t j = 0; j < 9; j++) {
					if (arrString[j] == 0) {
						arrString[j] = ' ';
					}
				}
				arrString[9] = 0;
				_writeData(); // Выводим строку «arrString» на на дисплей.
			}
		} // end _printNumS

		// Вывод массива:
		void _printNumA(int i[])
		{
			if (valAddr) { // Если модуль был инициализирован, то ...
				for (uint8_t j = 0; j < 9; j++) {
					arrString[j] = ' ';
				}
				arrString[9] = 0; //
				if (i[0] <= 16) {
					itoa(i[0], & arrString[0], HEX);
				} else {
					arrString[0] = ' ';
				}
				arrString[1] = ' '; //
				if (i[1] <= 16) {
					itoa(i[1], & arrString[2], HEX);
				} else {
					arrString[2] = ' ';
				}
				arrString[3] = ' ';
				arrString[4] = ' '; //
				if (i[2] <= 16) {
					itoa(i[2], & arrString[5], HEX);
				} else {
					arrString[5] = ' ';
				}
				arrString[6] = ' '; //
				if (i[3] <= 16) {
					itoa(i[3], & arrString[7], HEX);
				} else {
					arrString[7] = ' ';
				}
				arrString[8] = ' ';
				arrString[9] = 0; //
				_writeData(); //	Выводим строку «arrString» на на дисплей.
			} //
		} // end _printNumA

		// функции обработки строк:
		// вставить символ j в строку i, n раз, начиная с позиции k
		void _strInsert(char * i, char j, int n, int k)
		{
			if (n < 0 || k < 0) {
				return;
			}
			for (uint8_t l = 0; l < n; l++) {
				for (uint8_t m = strlen(i) + 1; m > k; m--) {
					i[m] = i[m - 1];
				}
				i[k] = j;
			}
		}

		// удалить n символов из строки i начиная с позиции k
		void _strDelete(char * i, int n, int k)
		{
			if (n < 0 || k < 0) {
				return;
			}
			for (uint8_t l = 0; l < n; l++) {
				for (uint8_t m = k; m <= strlen(i); m++) {
					i[m] = i[m + 1];
				}
			}
		}

		void _releaseAll()
		{
			for (auto& i:_dig)
				i.release();
		}

		// Указатель на массив объектов расширителей выводов
		iarduino_I2C_Expander* _E;

		// Пустые объекты цифр
		Digit _dig[DIGITS];

		// Количество цифр, если это не часы
		uint8_t _dig_num;
};

class Display: public MechaClock {
	public:
		using MechaClock::MechaClock;
};


#endif
