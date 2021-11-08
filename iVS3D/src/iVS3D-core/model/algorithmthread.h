#ifndef ALGORITHMTHREAD_H
#define ALGORITHMTHREAD_H

#include <QThread>
#include <opencv2/core.hpp>
#include "modelalgorithm.h"
#include "reader.h"
#include "plugin/algorithmmanager.h"
#include "logmanager.h"
#include "logfile.h"

/**
 * @class AlgorithmThread
 *
 * @ingroup model
 *
 * @brief The AlgorithmThread class is used to start the execution of a major plugin task on a new thread.
 *        The templated type will be used to define the type of output which is generated by the plugin task.
 *
 * @author Dominic Zahn
 *
 * @date 2021/02/08
 */
template<typename outType>
class AlgorithmThread : public QThread
{
public:
    /**
     * @brief AlgorithmThread Constructor which takes all parameters used by IAlgorithm
     * @param parameters according to IAlgorithm
     */
    AlgorithmThread(Progressable *receiver, Reader *reader, std::vector<uint> images, volatile bool *stopped, int pluginIdx, QMap<QString, QVariant> buffer, bool useCuda);
    /**
     * @brief getOutput getter for the computed output (for example keyframes or generated settings )
     * @return The datatype of the computed output is set by the template
     */
    outType getOutput();
    /**
     * @brief getResult Getter for the status of the plugin calulation
     * @returns 0 if plugin executed without errors, ny other number indicates an error
     */
    int getResult();
    /**
     * @brief getBuffer Getter for the buffer of the plugin after run was called.
     * @returns QVariant with the plugin buffer
     */
    QVariant getBuffer();
    /**
     * @brief getBufferName Getter for the bufferName of the plugin after run was called.
     * @returns QVaraint with the plugin buffer
     */
    QString getBufferName();

protected:
    /**
     * @brief QThread method stub which is implemented in its child classes
     */
    virtual void run() = 0;

    outType m_output;
    Progressable *m_receiver;
    int m_pluginIdx;
    Reader *m_reader;
    std::vector<uint> m_images = {};
    volatile bool *m_stopped;
    int m_result = -1;
    QMap<QString, QVariant> m_buffer;
    bool m_useCuda;
    LogFile *m_logFile;
};

#endif // ALGORITHMTHREAD_H
