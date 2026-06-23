#include <stdio.h>
#include <string.h>

/* 定義函數指標 */
#define GATEVALUE(Gate) int (*GateValue)(struct _Gate *)

typedef struct _Gate
{
    int input1;
    int input2;

    /* 儲存此邏輯閘所使用的函數 */
    GATEVALUE(Gate);
} Gate;


/* 只保留最低3位元，結果範圍為0~7 */
int mask3(int value){return value & 7;}
int GateORValue(Gate  *gate){return mask3(gate->input1 | gate->input2);}
int GateANDValue(Gate *gate){return mask3(gate->input1 & gate->input2);}
int GateNOTValue(Gate *gate){return mask3(~gate->input1);}
int GateBUFFERValue(Gate *gate){return mask3(gate->input1);}
int GateNANDValue(Gate *gate){return mask3(~(gate->input1 & gate->input2));}
int GateNORValue(Gate *gate){return mask3(~(gate->input1 | gate->input2));}

/* 建立邏輯閘並設定對應的函數指標 */
void CreateGate(Gate *obj, char type[], int input1, int input2)
{
    obj->input1 = input1;
    obj->input2 = input2;

         if (strcmp(type, "N") == 0){obj->GateValue = GateNOTValue;}
    else if (strcmp(type, "B") == 0){obj->GateValue = GateBUFFERValue;}
    else if (strcmp(type, "A") == 0){obj->GateValue = GateANDValue;}
    else if (strcmp(type, "O") == 0){obj->GateValue = GateORValue;}
    else if (strcmp(type, "NA") == 0){obj->GateValue = GateNANDValue;}
    else if (strcmp(type, "NO") == 0){obj->GateValue = GateNORValue;}
}

int main(void)
{
    int x1, x2, x3;
    int resultX1, resultX2, resultX3;
    int y, output;

    char typeX1[3];
    char typeX2[3];
    char typeX3[3];
    char typeY[3];
    char typeOutput[3];

    Gate gateX1;
    Gate gateX2;
    Gate gateX3;
    Gate gateY;
    Gate gateOutput;

    /* 輸入X1、X2、X3 */
    scanf("%d%d%d", &x1, &x2, &x3);

    /* 輸入五個邏輯閘種類 */
    scanf("%2s%2s%2s%2s%2s",typeX1, typeX2, typeX3,typeY, typeOutput);
    /* 處理X1 */
    CreateGate(&gateX1, typeX1, x1, 0);
    resultX1 = gateX1.GateValue(&gateX1);

    /* 處理X2 */
    CreateGate(&gateX2, typeX2, x2, 0);
    resultX2 = gateX2.GateValue(&gateX2);

    /* 處理X3 */
    CreateGate(&gateX3, typeX3, x3, 0);
    resultX3 = gateX3.GateValue(&gateX3);

    /* 使用處理後的X1和X2計算Y */
    CreateGate(&gateY, typeY, resultX1, resultX2);
    y = gateY.GateValue(&gateY);

    /* 使用Y和處理後的X3計算最終結果 */
    CreateGate(&gateOutput, typeOutput, y, resultX3);
    output = gateOutput.GateValue(&gateOutput);

    /* 將結果輸出成3位元二進位 */
    printf("%d%d%d\n",(output >> 2) & 1,(output >> 1) & 1,output & 1);
    return 0;
}