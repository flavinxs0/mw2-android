# MW2 Android & PC (fork)

[![Licença: GPL v3](https://img.shields.io/badge/Licen%C3%A7a-GPLv3-blue.svg)](LICENSE)
[![Release](https://img.shields.io/github/v/release/flavinxs0/mw2-android?color=blue)](https://github.com/flavinxs0/mw2-android/releases/latest)

*Call of Duty: Modern Warfare 2 (2009)* do Xbox 360, recompilado antes da execução para código nativo em **Android (ARM64)**, **Linux** e **Windows (x86-64)**.

Isto **não é um emulador**. As instruções PowerPC do Xbox 360 são traduzidas para C++ com o [XenonRecomp](https://github.com/hedge-dev/XenonRecomp) e compiladas direto em binários nativos.

English version: [README.en.md](README.en.md)

> **Importante:** este repositório não contém assets, executáveis nem código do jogo. Você precisa ter sua própria cópia legal do disco de *Call of Duty: Modern Warfare 2* (Xbox 360, versão 1.0.557, EUA ou Europa).

## Recursos

- Campanha e multiplayer, offline e online.
- Rede por Steam (convite pela lista de amigos) ou LAN / IP direto.
- Renderizador Vulkan com resolução 720p, 1080p, 1440p ou 4K e MSAA.
- Suporte a controles Xbox, PlayStation e genéricos (Bluetooth/USB), com vibração.
- No Android: controles de toque personalizáveis, launcher com instalação em um toque, cache de shaders, HUD de desempenho (FPS, CPU, temperatura, RAM) e carregamento de drivers Turnip para GPUs Adreno.
- Fallback Vulkan 1.1 e decodificador BC em CPU para GPUs Mali e drivers mais antigos.

## Requisitos

**Android**
- arm64-v8a (64 bits) e Android 10 ou superior (API 29+).
- Vulkan 1.1 ou superior (1.3 usa o caminho moderno).
- Cerca de 7 a 8 GB livres.
- ISO ou dump do jogo (Xbox 360, v1.0.557).

**PC (Windows e Linux)**
- Windows 10/11 ou Linux x86-64 recente.
- GPU com Vulkan 1.2+ e a imagem do disco (v1.0.557).

## Instalação no Android

1. Baixe o `app-campaign-release.apk` (ou `app-multiplayer-release.apk`) em [Releases](https://github.com/flavinxs0/mw2-android/releases/latest) e instale.
2. Copie sua ISO do jogo para a pasta `Download` do celular.
3. Abra o app e toque em **INSTALAR E JOGAR** (o launcher detecta a ISO). Quando terminar, toque em **INICIAR JOGO**.

No launcher você encontra Driver Gráfico, Gráficos (escala de render, limite de FPS, FOV) e Perfis.

## Compilar do código-fonte

Precisa de JDK 17, Android SDK com NDK 28.2.13676358 (`platforms;android-35`), CMake 3.22.1+ e Python 3.10+.

1. `TITLE=sp ./build.sh` e `TITLE=mp ./build.sh` (recompila o executável do jogo).
2. Opcional: `./android/fetch_deps.sh` (suporte a drivers Adreno custom).
3. `cd android && ./gradlew assembleCampaignTu0Release`

O APK sai em `android/app/build/outputs/apk/campaignTu0/release/`. Os workflows em `.github/workflows` também compilam pelo GitHub Actions.

## Solução de problemas

- **Logs (Android):** `/sdcard/Android/data/com.mw2.recomp.sp/files/game.log` e `launcher.log`.
- **Tela preta:** veja o `game.log`. Em Adreno, carregue um driver Turnip pelo menu Driver do app.
- **Desempenho:** use escala de render entre 60% e 75%, deixe o MSAA desligado e ajuste o orçamento de textura para 256 ou 512 MB.

## Jogando online com segurança

Os bugs de segurança conhecidos da rede de 2009 foram corrigidos (veja `docs/security.md`), mas o código continua antigo. Jogue online só com pessoas de confiança. Não existe matchmaking público.

## Créditos

Este fork é baseado no trabalho de outras pessoas, e a atribuição é mantida conforme a GPL-3.0:

- [OpenPort-Labs/mw2-android](https://github.com/OpenPort-Labs/mw2-android): o port para Android em que este fork se baseia.
- [PaulCombal/mw2-recompiled](https://github.com/PaulCombal/mw2-recompiled): projeto original de recompilação e arquitetura do renderizador Vulkan.
- [hedge-dev/XenonRecomp](https://github.com/hedge-dev/XenonRecomp): recompilador estático de PowerPC para C++.
- [KisakCOD](https://github.com/SwagSoftware/KisakCOD) e [IW4x](https://github.com/iw4x/iw4x-client): pesquisa sobre sistemas da engine, áudio e entrada.
- [OpenAssetTools](https://github.com/Laupetin/OpenAssetTools): ferramentas de formatos do Call of Duty.
- [libadrenotools](https://github.com/bylaws/libadrenotools): carregamento de drivers Vulkan custom no Android.

## Licença e aviso legal

Licenciado sob a **GNU GPL v3.0** (veja [LICENSE](LICENSE)).

Projeto independente de pesquisa e código aberto, sem afiliação, autorização ou endosso da Activision, Infinity Ward ou Microsoft. Todas as marcas e conteúdos do jogo pertencem aos seus donos. Nenhum asset ou binário protegido é distribuído aqui.
