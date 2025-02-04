"""
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
import oneflow as flow
from oneflow.framework.tensor import register_tensor_op
from oneflow.nn.module import Module


class Nms(Module):
    def __init__(self, iou_threshold: float) -> None:
        super().__init__()
        self.iou_threshold = iou_threshold

    def forward(self, boxes, scores):
        score_inds = flow.argsort(scores, dim=0, descending=True)
        boxes = flow._C.gather(boxes, score_inds, axis=0)
        keep = flow._C.nms(boxes, self.iou_threshold)
        index = flow.squeeze(flow.argwhere(keep), dim=[1])
        return flow._C.gather(score_inds, index, axis=0)


@register_tensor_op("nms")
def nms_op(boxes, scores, iou_threshold: float):
    """
    Performs non-maximum suppression (NMS) on the boxes according
    to their intersection-over-union (IoU).

    NMS iteratively removes lower scoring boxes which have an
    IoU greater than iou_threshold with another (higher scoring)
    box.

    Args:
        boxes (Tensor[N, 4]): boxes to perform NMS on. They
            are expected to be in ``(x1, y1, x2, y2)`` format with ``0 <= x1 < x2`` and
            ``0 <= y1 < y2``.
        scores (Tensor[N]): scores for each one of the boxes
        iou_threshold (float): discards all overlapping boxes with IoU > iou_threshold

    Returns:
        Tensor: int64 tensor with the indices of the elements that have been kept by NMS, sorted in decreasing order of scores
    """
    return Nms(iou_threshold=iou_threshold)(boxes, scores)
