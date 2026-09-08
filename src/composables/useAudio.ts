import { shallowRef, type ShallowRef } from 'vue'
import type {
  BgmSwitch,
  SfxSwitch,
  AudioUnlockState,
  BgmPlayState,
  SfxType
} from '@/domain/types'

const SFX_PATHS = {
  sfxPlace: '/audio/sfx-place.wav',
  sfxWin: '/audio/sfx-win.wav',
  sfxDraw: '/audio/sfx-draw.wav'
} as const

const BGM_INDEX_URL = '/bgm/index.json'
const BGM_DIR = '/bgm'

export interface AudioSnapshot {
  bgmSwitch: BgmSwitch
  volume: number
  sfxSwitch: SfxSwitch
  unlockState: AudioUnlockState
  bgmPlayState: BgmPlayState
  bgmAvailable: boolean
  sfxAvailable: Record<SfxType, boolean>
}

export interface UseAudio {
  snapshot: ShallowRef<AudioSnapshot>
  unlock: () => void
  toggleBgm: () => void
  setVolume: (value: number) => void
  toggleSfx: () => void
  onGameStart: () => void
  onGameEnd: () => void
  onGameReset: () => void
  playSfx: (type: SfxType) => void
}

function createInitialSnapshot(): AudioSnapshot {
  return {
    bgmSwitch: 'On',
    volume: 50,
    sfxSwitch: 'On',
    unlockState: 'Locked',
    bgmPlayState: 'Paused',
    bgmAvailable: true,
    sfxAvailable: { Place: true, Win: true, Draw: true }
  }
}

function shuffleArray<T>(arr: T[]): T[] {
  const result = [...arr]
  for (let i = result.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1))
    ;[result[i], result[j]] = [result[j], result[i]]
  }
  return result
}

export function useAudio(): UseAudio {
  const snapshot = shallowRef<AudioSnapshot>(createInitialSnapshot())

  const bgmEl = new Audio()
  bgmEl.loop = false
  const sfxElMap: Record<SfxType, HTMLAudioElement> = {
    Place: new Audio(SFX_PATHS.sfxPlace),
    Win: new Audio(SFX_PATHS.sfxWin),
    Draw: new Audio(SFX_PATHS.sfxDraw)
  }

  let trackList: string[] = []
  let shuffledQueue: string[] = []
  let queueIndex = 0
  let gameInProgress = false
  let switching = false

  for (const key of Object.keys(sfxElMap) as SfxType[]) {
    sfxElMap[key].addEventListener('error', () => {
      update({ sfxAvailable: { ...snapshot.value.sfxAvailable, [key]: false } })
    })
  }

  function update(partial: Partial<AudioSnapshot>): void {
    snapshot.value = { ...snapshot.value, ...partial }
  }

  async function loadTrackList(): Promise<void> {
    try {
      const resp = await fetch(BGM_INDEX_URL)
      if (!resp.ok) {
        update({ bgmAvailable: false })
        return
      }
      const data = await resp.json()
      if (!Array.isArray(data) || data.length === 0) {
        update({ bgmAvailable: false })
        return
      }
      trackList = data.filter((x: unknown) => typeof x === 'string' && x.length > 0)
      if (trackList.length === 0) {
        update({ bgmAvailable: false })
        return
      }
      reshuffle()
    } catch {
      update({ bgmAvailable: false })
    }
  }

  function reshuffle(): void {
    if (trackList.length === 0) return
    shuffledQueue = shuffleArray(trackList)
    queueIndex = 0
  }

  function playCurrentTrack(): void {
    if (shuffledQueue.length === 0) return
    if (queueIndex >= shuffledQueue.length) {
      reshuffle()
    }
    if (shuffledQueue.length === 0) return

    const trackName = shuffledQueue[queueIndex]
    const url = `${BGM_DIR}/${encodeURIComponent(trackName)}`
    switching = true
    bgmEl.src = url
    bgmEl.volume = snapshot.value.volume / 100
    bgmEl.play().then(() => {
      switching = false
      update({ bgmPlayState: 'Playing' })
    }).catch(() => {
      switching = false
      advanceToNext()
    })
  }

  function advanceToNext(): void {
    queueIndex++
    if (queueIndex >= shuffledQueue.length) {
      reshuffle()
    }
    if (snapshot.value.bgmSwitch === 'On' &&
        snapshot.value.unlockState === 'Unlocked' &&
        gameInProgress &&
        snapshot.value.bgmAvailable) {
      playCurrentTrack()
    }
  }

  bgmEl.addEventListener('ended', () => {
    advanceToNext()
  })

  bgmEl.addEventListener('error', () => {
    if (switching) return
    advanceToNext()
  })

  function applyBgmState(): void {
    const s = snapshot.value
    if (!s.bgmAvailable) return
    if (s.bgmSwitch === 'On' && s.unlockState === 'Unlocked' && gameInProgress) {
      if (bgmEl.paused || bgmEl.ended) {
        if (bgmEl.ended || !bgmEl.src) {
          playCurrentTrack()
        } else {
          bgmEl.volume = s.volume / 100
          bgmEl.play().catch(() => {})
          update({ bgmPlayState: 'Playing' })
        }
      }
    } else {
      bgmEl.pause()
      update({ bgmPlayState: 'Paused' })
    }
  }

  function unlock(): void {
    if (snapshot.value.unlockState === 'Unlocked') return
    update({ unlockState: 'Unlocked' })
    if (trackList.length === 0) {
      loadTrackList().then(() => {
        applyBgmState()
      })
    } else {
      applyBgmState()
    }
  }

  function toggleBgm(): void {
    const newSwitch: BgmSwitch = snapshot.value.bgmSwitch === 'On' ? 'Off' : 'On'
    update({ bgmSwitch: newSwitch })
    applyBgmState()
  }

  function setVolume(value: number): void {
    const clamped = Math.max(0, Math.min(100, Math.round(value)))
    bgmEl.volume = clamped / 100
    for (const key of Object.keys(sfxElMap) as SfxType[]) {
      sfxElMap[key].volume = clamped / 100
    }
    update({ volume: clamped })
  }

  function toggleSfx(): void {
    const newSwitch: SfxSwitch = snapshot.value.sfxSwitch === 'On' ? 'Off' : 'On'
    update({ sfxSwitch: newSwitch })
  }

  function onGameStart(): void {
    gameInProgress = true
    if (trackList.length === 0) {
      loadTrackList().then(() => {
        applyBgmState()
      })
    } else {
      applyBgmState()
    }
  }

  function onGameEnd(): void {
    gameInProgress = false
    applyBgmState()
  }

  function onGameReset(): void {
    gameInProgress = false
    applyBgmState()
  }

  function playSfx(type: SfxType): void {
    const s = snapshot.value
    if (s.sfxSwitch === 'Off') return
    if (!s.sfxAvailable[type]) return
    const el = sfxElMap[type]
    el.volume = s.volume / 100
    el.currentTime = 0
    el.play().catch(() => {})
  }

  loadTrackList()

  return {
    snapshot,
    unlock,
    toggleBgm,
    setVolume,
    toggleSfx,
    onGameStart,
    onGameEnd,
    onGameReset,
    playSfx
  }
}
