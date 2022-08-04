#include "colmapqueueitem.h"

// std
#include <stdio.h>

// Qt
#include <QPushButton>

#include "ui_colmapqueueitem.h"
#include "ui_colmapqueueitem_running.h"
#include "ui_colmapqueueitem_finished.h"

namespace lib3d {
namespace ots {
namespace ui {
namespace colmapwrapper {

//==================================================================================================
QueueItem::QueueItem(ColmapWrapper::SJob job, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueueItem)
{
    mJob = job;
    ui->setupUi(this);
    ui->l_name->setText(QString::fromStdString(job.sequenceName).
                        append(": ").append(ColmapWrapper::EProductType2QString(job.product)));

    //--- connect buttons to slots
    connect(ui->btnDown, &QPushButton::clicked, this, &QueueItem::onBtnDownClicked);
    connect(ui->btnUp, &QPushButton::clicked, this, &QueueItem::onBtnUpClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QueueItem::onBtnCancelClicked);
}

//==================================================================================================
QueueItem::~QueueItem()
{
  delete ui;
}

//==================================================================================================
void QueueItem::onUpdateToDarkTheme()
{
  ui->btnCancel->setIcon(QIcon(":/assets/icons/glyphicons-17-bin-dark.png"));
  ui->btnUp->setIcon(QIcon(":/assets/icons/glyphicons-370-collapse-top-dark.png"));
  ui->btnDown->setIcon(QIcon(":/assets/icons/glyphicons-368-expand-dark.png"));
}

//==================================================================================================
void QueueItem::onUpdateToLightTheme()
{
  ui->btnCancel->setIcon(QIcon(":/assets/icons/glyphicons-17-bin.png"));
  ui->btnUp->setIcon(QIcon(":/assets/icons/glyphicons-370-collapse-top.png"));
  ui->btnDown->setIcon(QIcon(":/assets/icons/glyphicons-368-expand.png"));
}

//==================================================================================================
void QueueItem::onBtnUpClicked()
{
    emit bumpUpJob(mJob);
}

//==================================================================================================
void QueueItem::onBtnDownClicked()
{
    emit bumpDownJob(mJob);
}

//==================================================================================================
void QueueItem::onBtnOptionsClicked()
{
    emit editJob(mJob);
}

//==================================================================================================
void QueueItem::onBtnCancelClicked()
{
    emit deleteJob(mJob);
}

//==================================================================================================
QueueItemActive::QueueItemActive(ColmapWrapper::SJob job, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueueItemRunning)
{
    this->job = job;
    ui->setupUi(this);
    ui->l_name->setText(QString::fromStdString(job.sequenceName).
                        append(": ").append(ColmapWrapper::EProductType2QString(job.product)));
}

//==================================================================================================
QueueItemActive::~QueueItemActive()
{
    delete ui;
}

//==================================================================================================
void QueueItemActive::setProgress(int progress) {
  ui->progressBar->setValue(progress);
}

//==================================================================================================
void QueueItemActive::onUpdateToDarkTheme()
{
  ui->btnCancel->setIcon(QIcon(":/assets/icons/glyphicons-17-bin-dark.png"));
}

//==================================================================================================
void QueueItemActive::onUpdateToLightTheme()
{
  ui->btnCancel->setIcon(QIcon(":/assets/icons/glyphicons-17-bin.png"));
}

//==================================================================================================
void QueueItemActive::onBtnOptionsClicked()
{
    emit cancel();
}

//==================================================================================================
QueueItemFinished::QueueItemFinished(ColmapWrapper::SJob job, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueueItemFinished)
{
  this->job = job;
  ui->setupUi(this);

  ui->l_name->setText(QString::fromStdString(job.sequenceName).
                      append(": ").append(ColmapWrapper::EProductType2QString(job.product)));
}

//==================================================================================================
QueueItemFinished::~QueueItemFinished()
{
  delete ui;
}

//==================================================================================================
void QueueItemFinished::onUpdateToDarkTheme()
{
  ui->btnDelete->setIcon(QIcon(":/assets/icons/glyphicons-17-bin-dark.png"));
}

//==================================================================================================
void QueueItemFinished::onUpdateToLightTheme()
{
  ui->btnDelete->setIcon(QIcon(":/assets/icons/glyphicons-17-bin.png"));
}

//==================================================================================================
void QueueItemFinished::onBtnDeleteClicked()
{
    emit deleteJob(job);
}

} // namespace colmapwrapper
} // namespace ui
} // namespace ots
} // namespeace lib3d