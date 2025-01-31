struct button
{
    char keycode;
    byte x;
    byte y;
    byte size;
    char near_keycode[4]; // up right down  left
};

byte currentKey = -1, anotherButton = 0;
const byte totalKeys = 35;
const button keyboard[]  =
{

    {0,14,0,12,{34,6,40,2,}},//CLR
    {40,28,0,12,{0,46,8,42,}},//TAN
    {8,42,0,12,{40,14,56,10,}},//RCL
    {56,56,0,12,{8,62,50,58,}},//CLX
    {50,66,0,16,{56,54,18,51,}},//9
    {18,80,0,16,{50,22,26,19,}},//6
    {26,94,0,16,{18,30,34,27,}},//3
    {34,108,0,16,{26,38,0,35,}},//PI
    {2,14,13,12,{35,0,42,3,}},//eX
    {42,28,13,12,{2,40,10,43,}},//COS
    {10,42,13,12,{42,8,58,11,}},//STO
    {58,56,13,12,{10,56,51,59,}},//EEX
    {51,66,17,16,{58,50,19,52,}},//8
    {19,80,17,16,{51,18,27,20,}},//5
    {27,94,17,16,{19,26,35,28,}},//2
    {35,108,17,16,{27,34,2,36,}},//DOT
    {3,14,26,12,{35,2,43,4,}},//ln
    {43,28,26,12,{3,42,11,44,}},//SIN
    {11,42,26,12,{43,10,59,12,}},//RV
    {59,56,26,12,{11,58,51,62,}},//CHS
    {52,66,34,16,{62,51,20,54,}},//7
    {20,80,34,16,{52,19,28,22,}},//4
    {28,94,34,16,{20,27,36,30,}},//1
    {36,108,34,16,{28,35,4,38,}},//0
    {4,14,39,12,{36,3,44,6,}},//log
    {44,28,39,12,{4,43,12,46,}},//ARC
    {12,42,39,12,{44,11,62,14,}},//XTOY
    {62,43,39,25,{14,59,54,56,}},//ENTER
    {54,69,51,13,{62,52,22,50,}},//-
    {22,83,51,13,{54,20,30,18,}},//+
    {30,97,51,13,{22,28,38,26,}},//M
    {38,111,51,13,{30,36,6,34,}},///
    {6,14,52,12,{38,4,46,0,}},//XEXPY
    {46,28,52,12,{6,44,14,40,}},//SQR
    {14,42,52,12,{46,12,62,8,}},//1/x


};
