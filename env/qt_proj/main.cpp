#include <QFile>
#include <QTextStream>
int getM(int, int);
void main(){

    //for(;;)
    int Mxp, Mxm, Myp, Mym, Mzp, Mzm; //управляющий импульс по двум каналам
    int Mx, My, Mz; //управляющий импульс
    int wx, wy, wz;	//вектор скорости
    int h; //шаг интегрирования


    Mx=getM(Mxp, Mxm);
    My=getM(Myp, Mym); //get Mx, My, Mzm
    Mz=getM(Mzp, Mzm);

    wx = Mx * h + wx;
    wy = My * h + wy;
    wz = Mz * h + wz;

//запись в файл	векторов скоростей
    QString filename = "..//Data.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << wx  << "\t" << wy << "\t" << wz << endl;
    }
}

int getM(int Mp, int Mm)
{
    if (Mp == 1)
        return 1;
    else if (Mm == 1)
        return -1;
    else
        return 0;
}
