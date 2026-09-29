#ifndef EDGELLAMA_INCLUDE_OP_ADD_H
#define EDGELLAMA_INCLUDE_OP_ADD_H
#include "base/base.h"
#include "layer.h"
namespace op {
class VecAddLayer : public Layer {
 public:
  explicit VecAddLayer(base::DeviceType device_type);

  base::Status check() const override;

  base::Status forward() override;
};
}  // namespace op
#endif  // EDGELLAMA_INCLUDE_OP_ADD_H