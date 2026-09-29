# EdgeLlama

EdgeLlama 是一个 C++ 大模型推理框架，包含 Llama 2/3、Qwen2.5 和 Qwen3 的模型实现，提供 CPU/CUDA 算子、模型导出工具及单元测试。

## 项目结构

```text
edgellama/
  include/       公共头文件：内存、张量、模型、算子和采样器
  source/        推理实现及 CPU/CUDA 算子
cmake/           构建配置和依赖管理
demo/            推理示例
test/            单元测试
tmp/test.bin     模型加载测试使用的二进制数据
tools/           模型转换和导出工具
hf_infer/        Hugging Face 推理对照脚本
dockerfile       基础依赖环境
```

## 构建依赖

当前构建配置使用 C++17、CUDA C++14，面向 Linux/CUDA 环境，默认 CUDA 编译器路径为 `/usr/local/cuda/bin/nvcc`。示例默认使用 CUDA 设备。

- CMake 3.16 或更高版本，以及 C++ 编译器和 CUDA Toolkit。
- Google glog、GoogleTest、SentencePiece、Armadillo 及 BLAS 后端。
- 启用 Llama 3 或 Qwen 支持时，还需要 Abseil、RE2 和 nlohmann/json。
- Python 导出及对照脚本按需使用 PyTorch、Transformers、NumPy、SentencePiece、tqdm 和 Accelerate。

`dockerfile` 提供基础构建依赖，CUDA Toolkit 需要另行配置。

## 编译

在项目根目录执行：

```shell
mkdir build
cd build
cmake ..
make -j16
cd ..
```

配置时可添加 `-DUSE_CPM=ON`，由 CPM 下载配置中的第三方依赖；此方式需要网络连接及相应系统依赖。

按模型选择构建选项，切换模型时建议使用独立构建目录：

| 模型 | CMake 选项 | 示例程序 |
| --- | --- | --- |
| Llama 2 | 默认配置 | `build/demo/llama_infer` |
| Llama 3 | `-DLLAMA3_SUPPORT=ON` | `build/demo/llama_infer` |
| Qwen2.5 | `-DQWEN2_SUPPORT=ON` | `build/demo/qwen_infer` |
| Qwen3 | `-DQWEN3_SUPPORT=ON` | `build/demo/qwen3_infer` |

## 模型导出和运行

先准备对应模型的权重及分词器文件，将下列路径替换为本地模型目录。各模型使用对应的导出脚本：

```shell
# Llama 2
python tools/export_llama.py llama2.bin --meta-llama models/llama2/7B
./build/demo/llama_infer llama2.bin models/llama2/tokenizer.model

# Llama 3：使用 LLAMA3_SUPPORT 构建
python tools/export_llama3.py llama3.bin --hf models/llama3
./build/demo/llama_infer llama3.bin models/llama3/tokenizer.json

# Qwen2.5：使用 QWEN2_SUPPORT 构建
python tools/export_qwen2.py qwen2.bin --hf models/qwen2
./build/demo/qwen_infer qwen2.bin models/qwen2/tokenizer.json

# Qwen3：使用 QWEN3_SUPPORT 构建
python tools/export_qwen3/load.py --model_name models/qwen3 --output_file qwen3_0.6b_weights.pth
python tools/export_qwen3/write_bin.py --model_name models/qwen3 --checkpoint qwen3_0.6b_weights.pth
./build/demo/qwen3_infer qwen0.6.bin models/qwen3/tokenizer.json
```

Qwen3 导出配置位于 `tools/export_qwen3/config.py`，需与所选模型匹配；二进制导出脚本当前输出 `qwen0.6.bin`。Llama/Qwen2 导出脚本的量化选项需与推理端的模型加载配置匹配。

## 测试

编译后在项目根目录运行，以便读取 `tmp/test.bin`：

```shell
./build/test/test_llm
```

CUDA 测试需要可用的 GPU 和运行环境。`hf_infer/` 中的脚本可用于对照模型推理结果。

第三方组件的许可证及版权声明保留在对应源文件中。
