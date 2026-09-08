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
| 增大音量 | `vol+`（步长 10，范围 0-100） | `vol+` |
| 减小音量 | `vol-`（步长 10，范围 0-100） | `vol-` |
| 静音切换 | `mute` | `mute` |
| 退出 | `quit` | `quit` |

## 背景音乐

游戏启动后自动从 `C:\Users\16072\Desktop\game\bgm` 目录随机抽取音乐文件播放，当前曲目播放完毕后自动续播下一曲，一轮播完后重新随机洗牌循环播放。

- 音量调节：运行中输入 `vol+` 增大音量、`vol-` 减小音量、`mute` 静音切换，步长 10，范围 0-100
- 音频格式：逐个尝试加载，无法加载的文件自动跳过，不预设格式白名单
- 降级行为：bgm 目录缺失、为空或音频子系统不可用时，游戏正常运行无背景音乐，不崩溃

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
├── BgmPlayer.h/.cpp  背景音乐播放器（pimpl 门面）
├── BgmPlayerInternal.h  背景音乐内部组件（VolumeController/PlaylistManager）
├── Log.h            公共日志宏
├── third_party/miniaudio.h  miniaudio 单头文件音频库
└── main.cpp         程序入口
test/
├── TestFramework.h     测试框架
├── TestMain.cpp        测试入口
├── TestBoard.cpp       Board 单元测试
├── TestJudge.cpp       Judge 单元测试
├── TestInputReader.cpp InputReader 单元测试
├── TestRenderer.cpp    Renderer 单元测试
├── TestIntegration.cpp 集成与端到端测试
├── TestVolumeController.cpp  音量控制单元测试
├── TestPlaylistManager.cpp   播放队列单元测试
└── TestBgmPlayer.cpp   背景音乐播放器集成测试
```

## 调试日志

编译时定义 `GOMOKU_DEBUG` 宏可开启运行日志：

```bash
cmake -B build -DGOMOKU_DEBUG=ON
cmake --build build
```
