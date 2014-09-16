

#ifndef GRABBER_H_
#define GRABBER_H_

#include "CaptureDevice.h"
#include "DeviceInterface.h"
#include "Properties.h"
#include "PipelineManager.h"

#include <string>
#include <vector>
#include <memory>

namespace tis_imaging
{

class Grabber
{
    
public:
    
    Grabber ();
    
    ~Grabber ();

    // device related:
    
    bool openDevice (const CaptureDevice&);

    bool isDeviceOpen () const;

    CaptureDevice getDevice() const;
    
    bool closeDevice ();

    // property related:

    std::vector<Property> getAvailableProperties ();

    // videoformat related:
    
    std::vector<VideoFormatDescription> getAvailableVideoFormats () const;

    bool setVideoFormat (const VideoFormat&);

    VideoFormat getActiveVideoFormat () const;

    // playback related:

    bool startStream (std::shared_ptr<ImageSink>);

    bool stopStream ();

private:

    std::shared_ptr<PipelineManager> pipeline;

    // GrabberImpl* impl;
    
    CaptureDevice open_device;

    // std::shared_ptr<Device> capture;
    std::shared_ptr<DeviceInterface> device;
    
    std::vector<std::shared_ptr<Property>> device_properties;
    
    std::vector<std::shared_ptr<Property>> pipeline_properties;
    
    std::vector<std::shared_ptr<Property>> user_properties;
    
}; /* class Grabber */

} /* namespace tis_imaging */

#endif /* GRABBER_H_ */
