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

from absl import flags

import oneflow.unittest

FLAGS = flags.FLAGS


class Test2NodeMixin:
    def test_2n8c(self):
        self.run_net(4, 2, FLAGS.nodes_list)
        self.print_and_check_result("2n8c")
