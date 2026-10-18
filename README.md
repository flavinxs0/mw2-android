Enter	START (Pause Menu / Skip Intro)
Esc, Tab	BACK (Scoreboard / Cancel)
Arrow Keys	D-Pad (Night Vision / Killstreaks)
F8	Toggle Fullscreen
F6	Invite Friends (Steam build)
Building from Source
Android APK Build
Prerequisites
Desktop environment (Linux or Windows).
JDK 17.
Android SDK with NDK 28.2.13676358 (platforms;android-35).
CMake 3.22.1+ and Python 3.10+.
# 1. Recompile Xbox 360 guest executables to C++
TITLE=sp ./build.sh
TITLE=mp ./build.sh

# 2. (Optional) Fetch libadrenotools for custom Adreno GPU driver support
./android/fetch_deps.sh

# 3. Build the Campaign APK via Gradle
cd android
./gradlew assembleCampaignTu0Release

# The output APK will be in:
# android/app/build/outputs/apk/campaignTu0/release/*.apk
Automated GitHub Actions CI
The .github/workflows/android.yml workflow builds installable APKs directly on GitHub runners:

Go to Actions → android → Run workflow.
Supply a download link to your legal game executables or ISO archive.
The runner recompiles the tree, runs the NDK toolchain with ccache, and attaches the signed APK as an artifact.
Troubleshooting & Diagnostics
Log Files:
Android runtime logs are mirrored to internal and external storage: /sdcard/Android/data/com.mw2.recomp.sp/files/game.log and launcher.log.
On PC: pass MW2_LOG_FILE=mw2.log or view the terminal output.
Black Screen on Android:
Check game.log. If the log reports vulkan: the device lacks dynamic rendering and extended dynamic state, your device's stock GPU driver lacks Vulkan 1.3 core features. For Adreno devices, use the in-app Driver menu to load a Turnip driver package.
Performance Tuning:
Open Settings inside the Android app:
Set Render Scale to 60% - 75% for solid 60 FPS on mid-range devices.
Keep Multisampling (MSAA) OFF on mobile tiled GPUs.
Set Texture Budget to 256 MB or 512 MB to prevent memory pressure.
Credits & Acknowledgments
PaulCombal/mw2-recompiled: Original Xbox 360 ahead-of-time recompilation project and Vulkan renderer architecture.
hedge-dev/XenonRecomp: PowerPC to C++ static recompiler.
KisakCOD & IW4x: Research into engine systems, audio mixing, and mobile input adaptations.
OpenAssetTools: Call of Duty FastFile and asset format tools.
libadrenotools: Custom Vulkan driver loading on Android.
License
This project is licensed under the GNU General Public License v3.0 (GPL-3.0-only).
See LICENSE.md for full terms, conditions, and third-party notices.

Disclaimer: This project is an independent open-source research initiative. It is not affiliated with, authorized, or endorsed by Activision, Infinity Ward, or Microsoft. All trademarks and game content belong to their respective owners. No copyrighted game assets or proprietary binaries are hosted in this repository.
