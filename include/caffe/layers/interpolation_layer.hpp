/*
# Interpolation
#
# Author: Sun Yao @ IIE, CAS
# Create on: 2016-09-02
# Last modify: 2016-12-29
#
*/

#ifndef CAFFE_INTERPOLATION_LAYER_HPP_
#define CAFFE_INTERPOLATION_LAYER_HPP_

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"


namespace caffe {

/**
 */
template <typename Dtype>
class InterpolationLayer: public Layer<Dtype> {
 public:
  /**
   * @param param provides InflationFactorParameter inflation_factor_param,
   *     with InflationLayer options:
   *   - factor (\b optional, initial value for inflation factor,
   *     default {'type': constant 'value':1}).
   */
  explicit InterpolationLayer(const LayerParameter& param)
      : Layer<Dtype>(param), MAX_FACTOR(18), MIN_FACTOR(0.1) {};

  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "Interpolation"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  /**
   * @param bottom input Blob vector (length 1)
   * @param top output Blob vector (length 1)
   */
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the Inflation layer input and the factor.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   * @param propagate_down is unuseful in this implementation
   * @param bottom input Blob vector (length 1)
   */
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  
  /**
   * @brief Un ultility function that implements a bilinear interpolation. Different
   *   from the implementation of imresize in other libs, such as 
   *   scipy.misc.imresize or skimage.transform.resize, here we assume that 
   *   the coordinate is on the left-top of a pixel, rather than at the 
   *   pixel's center.
   * Thus, the inflated feature size if [(original size - 1) * factor + 1], 
   *   rather than (orignal size * factor).
   *
   * @param feature_in, feature_out are feature maps before and after inflation
   * @param old_height, old_width are the shape of input feature
   * @param new_height, new_width are the shape of output feature
   * @param factor controls the output feature's shape at this inflation operation
   */
  void inflate_forward(const Dtype *bottom_data, const int bottom_height, const int bottom_width, Dtype *top_data, const int top_height, const int top_width, const float factor_h, const float factor_w);
  void inflate_backward(Dtype *bottom_diff, const int bottom_height, const int bottom_width, const Dtype *top_diff, const int top_height, const int top_width, const float factor_h, const float factor_w);

  // upper and lower bounds for factor
  const float MAX_FACTOR;
  const float MIN_FACTOR;
  
  Dtype factor_value_h_;
  Dtype factor_value_w_;  
};

}  // namespace caffe

#endif  // CAFFE_INTERPOLATION_LAYER_HPP_
