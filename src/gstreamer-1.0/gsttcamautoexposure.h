/*
 * Copyright 2013 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GST_TCAMAUTOEXPOSURE_H_
#define _GST_TCAMAUTOEXPOSURE_H_

#include <gst/base/gstbasetransform.h>
#include "bayer.h"

#include "image_sampling.h"


#ifdef __cplusplus
extern "C"
{
#endif

G_BEGIN_DECLS

#define GST_TYPE_TCAMAUTOEXPOSURE            (gst_tcamautoexposure_get_type())
#define GST_TCAMAUTOEXPOSURE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_TCAMAUTOEXPOSURE, GstTcamautoexposure))
#define GST_TCAMAUTOEXPOSURE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_TCAMAUTOEXPOSURE, GstTcamautoexposureClass))
#define GST_IS_TCAMAUTOEXPOSURE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_TCAMAUTOEXPOSURE))
#define GST_IS_TCAMAUTOEXPOSURE_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_TCAMAUTOEXPOSURE))

/* helper structs */

typedef struct
{
    gdouble min;
    gdouble max;
    gdouble value;

} Gain;

typedef struct
{
    gdouble min;
    gdouble max;
    gdouble value;

} Exposure;


typedef unsigned char byte;

static const guint dist_mid = 100;
/* reference value for "optimal" brightness */
static const guint ref_val = 128;
static const gdouble steps_to_double_brightness = 3.0;

/* names of gstreamer elements used for camera interaction */
static const char* CAMERASRC_NETWORK = "GstAravis";
static const char* CAMERASRC_USB = "GstV4l2Src";
static const char* CAMERASRC_TCAM = "GstTcam";



typedef struct GstTcamautoexposure
{
    GstBaseTransform base_tcamautoexposure;

    GstPad *sinkpad;
    GstPad *srcpad;

    gboolean auto_exposure;
    gboolean auto_gain;


    Gain gain;
    Gain default_gain_values;
    Exposure exposure;
    Exposure default_exposure_values;

    GstElement* camera_src;
    tBY8Pattern pattern;
    format color_format;

    gst_tcam_image_size image_size;

    gint framerate_numerator;
    gint framerate_denominator;

    gint frame_counter;

} GstTcamautoexposure;

typedef struct GstTcamautoexposureClass
{
    GstBaseTransformClass base_tcamautoexposure_class;
} GstTcamautoexposureClass;

GType gst_tcamautoexposure_get_type (void);

G_END_DECLS


#ifdef __cplusplus
}
#endif


#endif
