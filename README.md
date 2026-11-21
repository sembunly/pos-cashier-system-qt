# POS System

Desktop POS for macOS built with C++20, Qt 6 Widgets, Qt SQL/SQLite, and CMake.

## Requirements

- CMake 3.21+
- C++20 compiler (Xcode command line tools)

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

## Clean build files

```bash
rm -rf build
```

## First login

The first launch creates a local SQLite database and one administrator account:

```text
Username: admin
Password: 1
```

This weak password is for local development only; change it before deployment. Passwords are salted and iteratively SHA-256 hashed; they are never stored as plaintext.

## Included workflows

- Role-aware sign-in (admin/cashier)
- Product creation, search/SKU lookup, soft delete, and stock adjustment history
- Cash cart checkout, tax, discount-ready totals, non-negative stock enforcement, atomic sale creation, and refunds
- Dashboard, sales ledger, CSV sales export, store settings, database backup, and receipt PDF generation
- Core tests covering total calculation, checkout stock changes, and refunds

## Database

In this CLion project the app database is `database/pos_cashier.sqlite`. The schema has migrations, foreign keys, constraints, and indexes. Products are soft-deleted with `is_active = 0`; sales are retained and refunds transition their status rather than deleting history.

Generated receipt PDFs are saved in `database/receipts/`.

## Tests

```bash
ctest --test-dir build/debug --output-on-failure
```
