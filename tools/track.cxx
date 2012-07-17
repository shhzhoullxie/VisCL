/*ckwg +5
 * Copyright 2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <vcl_iostream.h>

#include <vil/vil_image_view.h>
#include <vil/vil_load.h>
#include <vil/vil_save.h>
#include <vil/vil_convert.h>

#include "cl_manager.h"
#include "cl_task_registry.h"
#include "hessian.h"
#include "BRIEF.h"
#include "track_descr_match.h"

int main(int argc, char *argv[])
{
  cl_manager::inst()->report_system_specs();

  vil_image_view<vxl_byte> img1_color = vil_load(argv[1]);
  vil_image_view<vxl_byte> img2_color = vil_load(argv[2]);
  vil_image_view<vxl_byte> img3_color = vil_load(argv[3]);

  vil_image_view<vxl_byte> img1, img2, img3, output;
  vil_convert_planes_to_grey(img1_color, img1);
  vil_convert_planes_to_grey(img2_color, img2);
  vil_convert_planes_to_grey(img3_color, img3);

  try
  {
    track_descr_match_t tracker = NEW_VISCL_TASK(track_descr_match);

    vcl_vector<vnl_vector_fixed<double, 2> > kpts1, kpts2, kpts3;
    tracker->first_frame(img1, kpts1);
    vcl_vector<int> indices21(tracker->track(img2, kpts2, 100));
    vcl_vector<int> indices32(tracker->track(img3, kpts3, 100));

    write_tracks_to_file("tracks.txt", kpts2, kpts3, indices32);
  }
  catch(const cl::Error &e)
  {
    vcl_cerr << "ERROR: " << e.what() << " (" << e.err() << " : "
             << print_cl_errstring(e.err()) << ")" << vcl_endl;
    return 1;
  }
  return 0;
}


