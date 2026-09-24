# Morph testing notebook

A static four-page course: overview, Catch2/CTest setup, assertions, and real
file IO. The raycasting course has been replaced, including its examples.
The engine and root test setup are independent of this tutorial.

## Preview

From the repository root:

```sh
python3 -m http.server 4173 --directory docs
```

Open http://localhost:4173. No site build, external fonts, or frontend packages
are needed. Lessons and downloads work without JavaScript; the assertion lab
is a labeled JavaScript simulation, not a C++ compiler.

## Verify

```sh
python3 docs/tests/check_site.py
cmake -S docs/examples -B build-tutorial -DCMAKE_BUILD_TYPE=Debug
cmake --build build-tutorial --parallel 2
ctest --test-dir build-tutorial --output-on-failure
ctest --test-dir build-tutorial --parallel 4 --output-on-failure
```

First configure fetches pinned Catch2 3.8.1. Multi-configuration generators
need --config Debug for the build and -C Debug for CTest. Expect eight cases.
Try all assertion-lab choices, copy code, navigate with the keyboard, download
the ZIP, and check a narrow mobile viewport. The corrupted payload should pass
a size-only assertion but fail equality.

## Edit

Edit the HTML directly. Shared presentation lives in assets/style.css and
browser interactions in assets/site.mjs. Keep snippets synchronized with the
files in examples/. Rebuild the archive after editing examples:

```sh
python3 docs/tests/check_site.py --pack
```

The sample IO implementation is a documented snapshot; it deliberately retains
the current writer's unchecked write/close limitation. The tutorial does not
install Catch2 into the engine or modify the production API.

## Publish

The existing .github/workflows/pages.yml uploads docs on a push to main or a
manual run of “Publish tutorial notebook”. In repository Settings → Pages,
use GitHub Actions as the source. Internal links remain relative for project
subpaths. Editing these files locally does not deploy the site.
