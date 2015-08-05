


#ifndef TCAM_V4L2DEVICE_H
#define TCAM_V4L2DEVICE_H

#include "DeviceInterface.h"
#include "VideoFormat.h"
#include "VideoFormatDescription.h"

#include <linux/videodev2.h>
#include <memory>
#include <thread>

#pragma GCC visibility push (internal)

namespace tcam
{

class V4l2Device : public DeviceInterface
{

    struct property_description
    {
        int id; // v4l2 identification
        std::shared_ptr<Property> prop;
    };
    static const int EMULATED_PROPERTY = -1;

    class V4L2PropertyHandler : public PropertyImpl
    {
        friend class V4l2Device;

    public:
        V4L2PropertyHandler (V4l2Device*);


        std::vector<std::shared_ptr<Property>> create_property_vector ();

        bool set_property (const Property&);
        bool get_property (Property&);

    protected:

        static const int EMULATED_PROPERTY = -1;

        std::vector<property_description> properties;
        std::vector<property_description> special_properties;

        V4l2Device* device;
    };


public:

    explicit V4l2Device (const DeviceInfo&);

    V4l2Device () = delete;

    ~V4l2Device ();

    DeviceInfo getDeviceDescription () const;

    // TODO: eval if weak_ptr should be distributed
    std::vector<std::shared_ptr<Property>> getProperties ();

    bool set_property (const Property&);

    bool get_property (Property&);

    bool setVideoFormat (const VideoFormat&);

    bool validateVideoFormat (const VideoFormat&) const;

    VideoFormat getActiveVideoFormat () const;

    std::vector<VideoFormatDescription> getAvailableVideoFormats ();

    bool setFramerate (double framerate);

    double getFramerate ();

    bool setSink (std::shared_ptr<SinkInterface>);


    bool initialize_buffers (std::vector<std::shared_ptr<MemoryBuffer>>);

    bool release_buffers ();

    bool start_stream ();

    bool stop_stream ();


private:

    std::thread work_thread;

    DeviceInfo device;

    int fd;

    VideoFormat active_video_format;

    std::vector<VideoFormatDescription> available_videoformats;
    bool emulate_bayer;
    uint32_t emulated_fourcc;

    // v4l2 uses fractions
    // to assure correct handling we store the values received by v4l2
    // to reuse them when necessary
    struct framerate_conv
    {
        double fps;
        unsigned int numerator;
        unsigned int denominator;
    };

    std::vector<framerate_conv> framerate_conversions;

    std::shared_ptr<V4L2PropertyHandler> property_handler;

    /**
     * @brief iterate over all v4l2 format descriptions and convert them
     *        into the internal representation
     */
    void index_formats ();

    std::vector<double> index_framerates (const struct v4l2_frmsizeenum& frms);

    void determine_active_video_format ();

    void create_emulated_properties ();

    void index_all_controls (std::shared_ptr<PropertyImpl> impl);

    int index_control (struct v4l2_queryctrl* qctrl, std::shared_ptr<PropertyImpl> impl);

    void add_control (struct v4l2_queryctrl* queryctrl,
                      struct v4l2_ext_control* ctrl,
                      std::shared_ptr<PropertyImpl> impl);

    bool propertyChangeEvent (const Property&);

    bool changeV4L2Control (const property_description&);

    // streaming related

    bool is_stream_on;
    struct tcam_stream_statistics statistics;
    size_t current_buffer;
    std::vector<std::shared_ptr<MemoryBuffer>> buffers;

    std::shared_ptr<SinkInterface> listener;

    void stream ();

    bool get_frame ();

    void init_mmap_buffers ();

    void free_mmap_buffers ();

    tcam_image_size get_sensor_size () const;
};

} /* namespace tcam */

#pragma GCC visibility pop

#endif /* TCAM_V4L2DEVICE_H */
