# 五子棋游戏（C++）

本地双人对弈五子棋命令行游戏，仅依赖 C++17 标准库。

## 编译与运行

### 使用 CMake

```bash
cmake -B build
cmake --build build
```

运行游戏：

```bash
./build/gomoku        # Linux/macOS
build\Debug\gomoku.exe  # Windows MSVC
```

运行测试：

```bash
./build/gomoku_test
```

### 直接使用 g++

```bash
g++ -std=c++17 -Wall -Wextra -o gomoku src/main.cpp src/Board.cpp src/Judge.cpp src/Renderer.cpp src/InputReader.cpp src/GameController.cpp -Isrc
./gomoku
```

## 操作说明

| 操作 | 输入格式 | 示例 |
|------|---------|------|
| 落子 | `行 列`（空格分隔的两个整数） | `7 8` |
| 悔棋 | `undo` | `undo` |
| 退出 | `quit` | `quit` |

## 棋盘配置

- 尺寸范围：5 至 19 之间的奇数
- 默认尺寸：15×15
- 黑方（`X`）先手，白方（`O`）后手
- 空位显示为 `+`

## 胜负规则

- 双方轮流落子，率先在横、竖、斜任一方向形成五子连珠者获胜
- 支持长连（连珠数 ≥ 5 即获胜）
- 不设禁手规则
- 棋盘填满且无五子连珠则判平局

## 模块结构

```
src/
├── Types.h          公共类型定义（枚举与值类型）
├── Board.h/.cpp     棋盘表示模块
├── Judge.h/.cpp     规则判定模块（胜负/平局）
├── Renderer.h/.cpp  文本渲染模块
├── InputReader.h/.cpp  输入解析模块
├── GameController.h/.cpp  流程控制模块
└── main.cpp         程序入口
test/
├── TestFramework.h     测试框架
├── TestMain.cpp        测试入口
├── TestBoard.cpp       Board 单元测试
├── TestJudge.cpp       Judge 单元测试
├── TestInputReader.cpp InputReader 单元测试
├── TestRenderer.cpp    Renderer 单元测试
└── TestIntegration.cpp 集成与端到端测试
```

## 调试日志

编译时定义 `GOMOKU_DEBUG` 宏可开启运行日志：

```bash
cmake -B build -DCMAKE_CXX_FLAGS="-DGOMOKU_DEBUG"
cmake --build build
```
