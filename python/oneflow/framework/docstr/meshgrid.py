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
import oneflow
from oneflow.framework.docstr.utils import add_docstr

add_docstr(
    oneflow.meshgrid,
    """The interface is consistent with PyTorch.
    The documentation is referenced from:
    https://pytorch.org/docs/stable/_modules/torch/functional.html#meshgrid
    
    Take :math:`N` tensors, each of which can be either scalar or 1-dimensional
    vector, and create :math:`N` N-dimensional grids, where the :math:`i` :sup:`th` grid is defined by
    expanding the :math:`i` :sup:`th` input over dimensions defined by other inputs.

    Args:
        tensors (list of Tensor): list of scalars or 1 dimensional tensors. Scalars will be
            treated as tensors of size :math:`(1,)` automatically

    Returns:
        seq (sequence of Tensors): If the input has :math:`k` tensors of size
        :math:`(N_1,), (N_2,), \\ldots , (N_k,)`, then the output would also have :math:`k` tensors,
        where all tensors are of size :math:`(N_1, N_2, \\ldots , N_k)`.

    For example:

    .. code-block:: python

        >>> import numpy as np
        >>> import oneflow as flow
        
        >>> input1 = flow.tensor(np.array([1, 2, 3]), dtype=flow.float32)
        >>> input2 = flow.tensor(np.array([4, 5, 6]), dtype=flow.float32)
        >>> of_x, of_y = flow.meshgrid(input1, input2)
        >>> of_x
        tensor([[1., 1., 1.],
                [2., 2., 2.],
                [3., 3., 3.]], dtype=oneflow.float32)
        >>> of_y
        tensor([[4., 5., 6.],
                [4., 5., 6.],
                [4., 5., 6.]], dtype=oneflow.float32)
    """,
)
