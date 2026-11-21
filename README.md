# POS Cashier System — Phase 1

A beginner-friendly Qt 6 Widgets desktop POS dashboard for macOS.

## Requirements

- CMake 3.21+
- C++20 compiler (Xcode command line tools)
- Existing Qt 6.8.3 at `~/.local/share/system-monitor-qt/6.8.3/macos`

No Qt installation is required or performed by this project.

## Build from CLion

Open this folder as a CMake project. In **Settings → Build, Execution, Deployment → CMake**, add this CMake option if CLion does not discover Qt automatically:

```text
-DPOS_QT_DIR=$HOME/.local/share/system-monitor-qt/6.8.3/macos
```

Build and run the `pos_cashier_system` target.

## Build from Terminal

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
open build/debug/pos_cashier_system.app
```

## Database

On first launch the app creates a SQLite database in the standard macOS app-local data directory. The `products` table stores USD prices as integer `price_cents`, preventing floating-point currency errors. Products are soft-deleted by setting `is_active` to `0`; active-product queries should include `WHERE is_active = 1`.
