#ifndef STATIONARYCAMERA_H
#define STATIONARYCAMERA_H

/** @defgroup StationaryCameraPlugin StationaryCameraPlugin
 *
 * @ingroup Plugin
 *
 * @brief Plugin to remove keyframes if no camera movement was detected.
 */

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <iomanip>
#include <QtConcurrent/QtConcurrentMap>
#include <QThreadPool>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

#include "IAlgorithm.h"
#include "reader.h"
#include "factory.h"
#include "imagegatherer.h"
#include "flowcalculator.h"
#include <opencv2/video.hpp>

#define PLUGIN_NAME "Stationary Camera Detection"
// widget
#define THRESHOLD_LABEL_TEXT "Stationary camera threshold"
#define DESCRIPTION_THRESHOLD "If the rotation between two frames differs more than the defind percentage of the median rotation in the given frame sequence it is declared stationary."
#define DOWNSAMPLE_LABEL_TEXT "Down sampling factor"
#define DESCRIPTION_DOWNSAMPLE "The factor defines if input pictures are scaled down before computation. A higher factor results in a higher computation speed, but hurts accuracy."
#define DESCRIPTION_STYLE "color: rgb(58, 58, 58); border-left: 6px solid  rgb(58, 58, 58); border-top-right-radius: 5px; border-bottom-right-radius: 5px; background-color: lightblue;"
#define INFO_STYLE "color: rgb(58, 58, 58); border-left: 6px solid  rgb(58, 58, 58); border-top-right-radius: 5px; border-bottom-right-radius: 5px; background-color: lightGreen;"
#define INFO_PREFIX "Images are resized to "
#define INFO_SUFFIX " before computation."
// buffer
#define BUFFER_NAME "StationaryCameraMovementValues"
#define DELIMITER_COORDINATE "|"
#define DELIMITER_ENTITY ","
// settings
#define SETTINGS_THRESHOLD "Stationary threshold"
#define SETTINGS_DOWNSAMPLE "Downsample factor"
// log file
#define LF_BUFFER "Buffer"
#define LF_OPT_FLOW_TOTAL "Flow calculation"
#define LF_SELECT_FRAMES "Selection of keyframes"
#define LF_CE_TYPE_ADDITIONAL_INFO "Additional Computation Information"
#define LF_CE_VALUE_USED_BUFFERED "Used buffered values"
#define LF_CE_TYPE_DEBUG "Debug Information"
#define LF_CE_NAME_FLOWVALUE "Flow value"

/**
 * @class StationaryCamera
 *
 * @ingroup StationaryCameraPlugin
 *
 * @brief The StationaryCamera class implements the IAlgorithm interface and provides an algorithm to remove keyframes at positions where no movement of the camera was detected
 *
 * @author Dominic Zahn
 *
 * @date 2022/3/13
 */
class StationaryCamera : public IAlgorithm
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "pse.iVS3D.IAlgorithm")   // implement interface as plugin, use the iid as identifier
    Q_INTERFACES(IAlgorithm)                        // declare this as implementation of IAlgorithm interface

public:
    /**
     * @brief StationaryCamera Constructor sets default values for member variables
     */
    StationaryCamera();

    /**
     * @brief getSettingsWidget creates a Widget, which can be used to change the algorithm parameters and returns it
     * @param parent is the parent of the newly created SettingsWidget
     * @return a settingWidget, which can be used to change the algorithm parameters
     */
    QWidget *getSettingsWidget(QWidget *parent) override;

    /**
     * @brief sampleImages selects keyframe if the camera is currently not stationary
     * @param reader gives the method access to the video/image sequence, which should be used
     * @param imageList is a preselection of frames
     * @param receiver is a progressable, which displays the already made progress
     * @param stopped Pointer to a bool indication if user wants to stop the computation
     * @param buffer is a QVariant, which holds previous computions that could be usefull for the next selection
     * @param useCuda defines if the compution should run on graphics card
     * @param logFile poiter to the log file
     * @return A list of indices, which represent the selected keyframes.
     */
    std::vector<uint> sampleImages(Reader *reader, const std::vector<unsigned int> &imageList, Progressable *receiver, volatile bool *stopped, QMap<QString, QVariant> buffer, bool useCuda, LogFileParent *logFile) override;

    /**
     * @brief getName Returns a name for displaying this algorithm to the user.
     * @return the name as QString.
     */
    QString getName() const override;

    /**
     * @brief getBuffer Returns a buffer for storeing previously calculated Infos
     * @return the buffer as a QVariant (empty)
     */
    QVariant getBuffer() override;

    /**
     * @brief getBufferName Returns the name of the buffer
     * @return the name of the Buffer as a QString
     */
    QString getBufferName() override;

    /**
     * @brief initialize Sets up the default value which is corresponding to video information
     * @param reader is used to get video or image information
     */
    void initialize(Reader* reader) override;

    /**
     * @brief setter for plugin's settings
     * @param QMap with the settings
     */
    void setSettings(QMap<QString, QVariant> settings) override;

    /**
     * @brief generateSettings tries to generate the best settings for the current input
     * @param receiver is a progressable, which displays the already made progress
     * @param buffer QVariant with the buffered data form last call to sampleImages
     * @param useCuda @a true if cv::cuda can be used
     * @param stopped is set if the algorithm should abort
     * @return QMap with the settings
     */
    QMap<QString, QVariant> generateSettings(Progressable *receiver, QMap<QString, QVariant> buffer, bool useCuda, volatile bool* stopped) override;

    /**
     * @brief getter for plugin's settings
     * @return QMap with the settings
     */
    QMap<QString, QVariant> getSettings() override;

private:
    // member variables
    double m_threshold = 0.1;
    double m_downSampleFactor = 1.0;
    Reader *m_reader = nullptr;
    QPoint m_inputResolution = QPoint(0, 0);
    LogFileParent *m_logFile = nullptr;
    cv::SparseMat m_bufferMat;
    //      widget elements
    QWidget *m_settingsWidget = nullptr;
    QDoubleSpinBox *m_thresholdSpinBox = nullptr;
    QDoubleSpinBox *m_downSampleSpinBox = nullptr;
    QLabel *m_infoLabel = nullptr;
    // timing variables
    long m_durationFarnebackMs = 0;
    long m_durationComputationFlowMs = 0;

    // functions
    void reportProgress(QString op, int progress, Progressable *receiver);
    void createSettingsWidget(QWidget *parent);
    void updateInfoLabel();
    /**
     * @brief recreateBufferMatrix initalizes the buffer matix whith the new values from nBuffer
     * @param buffer holds the new movement values which should be stored in the buffer matrix
     */
    void recreateBufferMatrix(QMap<QString, QVariant> buffer);
    /**
     * @brief recreateMovementFromString is used to extract the movement value from a string and write it in the buffer
     * @param string that holds the two compared images indices and the movement value (most likeyl from settings.json)
     */
    void stringToBufferMat(QString string);
    QVariant bufferMatToVariant(cv::SparseMat bufferMat);
    double median(std::vector<double> &vec);
};

#endif // STATIONARYCAMERA_H
