#!/bin/bash

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"
SRC_DIR="${PROJECT_DIR}/src"
BIN="${BUILD_DIR}/pywgl_tabbed"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  🚀 pywgl_tabbed Development - Hot-Reload Enabled         ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

cd "${PROJECT_DIR}"
bash build.sh

restart_app() {
	if pgrep -f "pywgl_tabbed" > /dev/null 2>&1; then
		pkill -f "pywgl_tabbed" || true
		sleep 0.3
	fi
	cd "${PROJECT_DIR}"
	"${BIN}" > /tmp/pywgl_tabbed.log 2>&1 &
	APP_PID=$!
	echo "▶️  App running (PID: $APP_PID)"
}

rebuild() {
	cd "${BUILD_DIR}"
	if make -j$(nproc) 2>&1 | grep -q "error"; then
		echo "❌ Build failed"
		return 1
	fi
	echo "✅ Build successful"
	return 0
}

if ! command -v inotifywait &> /dev/null; then
	echo "⚠️  inotify-tools not found. Run without hot-reload."
	restart_app
	wait
	exit 0
fi

restart_app

echo ""
echo "👀 Watching for changes... (Ctrl+C to stop)"
echo ""

while true; do
	inotifywait -e modify -r "${SRC_DIR}" 2>/dev/null --exclude "\.swp|\.tmp" | while read -r _ _ file; do
		if [[ "$file" == *.cpp ]] || [[ "$file" == *.hpp ]] || [[ "$file" == CMakeLists.txt ]]; then
			echo "📝 Changed: $file"
			if rebuild; then
				restart_app
			fi
		fi
	done
done
