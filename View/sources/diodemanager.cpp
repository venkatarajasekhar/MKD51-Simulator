#include "../headers/diodemanager.h"
#include "../headers/diode.h"
#include <QHBoxLayout>

namespace View{

DiodeManager::DiodeManager(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<uint8_t>("uint8_t");
    QHBoxLayout* diodesLayout= new QHBoxLayout;

    for(int iter=0; iter<m_redDiodes; iter++)
    {
        Diode* newDiode= new Diode(0,Qt::red, false,2);
        diodesLayout->addWidget(newDiode);
        m_diodes.append(newDiode);
    }

    for(int iter=0; iter<m_yellowDiodes; iter++)
    {
        Diode* newDiode = new Diode(0,Qt::yellow, false, 2);
        diodesLayout->addWidget(newDiode);
        m_diodes.append(newDiode);
    }

    setLayout(diodesLayout);
}

void DiodeManager::changeDiodesState(int diodeOffset, bool newStatus)
{
    if(diodeOffset >= m_diodes.size())
        return;

    m_diodes.at(diodeOffset)->changeStatus(newStatus);
}

void DiodeManager::changeDiodesState(uint8_t newDiodesStatus)
{
    const int bitsInByte=8;
    for(int currentOffset=0; currentOffset<bitsInByte; currentOffset++)
    {
        if(newDiodesStatus & 1>>currentOffset)
            changeDiodesState(currentOffset, true);
        else
            changeDiodesState(currentOffset, false);
    }

}

DiodeManager::~DiodeManager()
{

}

}
