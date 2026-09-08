import { shallowRef, type ShallowRef } from 'vue'
import type {
  BgmSwitch,
  SfxSwitch,
  AudioUnlockState,
  BgmPlayState,
  SfxType
} from '@/domain/types'

const AUDIO_PATHS = {
  bgm: '/audio/bgm.wav',
  sfxPlace: '/audio/sfx-place.wav',
  sfxWin: '/audio/sfx-win.wav',
  sfxDraw: '/audio/sfx-draw.wav'
} as const

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

export function useAudio(): UseAudio {
  const snapshot = shallowRef<AudioSnapshot>(createInitialSnapshot())

  const bgmEl = new Audio(AUDIO_PATHS.bgm)
  bgmEl.loop = true
  const sfxElMap: Record<SfxType, HTMLAudioElement> = {
    Place: new Audio(AUDIO_PATHS.sfxPlace),
    Win: new Audio(AUDIO_PATHS.sfxWin),
    Draw: new Audio(AUDIO_PATHS.sfxDraw)
  }

  bgmEl.addEventListener('error', () => {
    update({ bgmAvailable: false })
  })
  for (const key of Object.keys(sfxElMap) as SfxType[]) {
    sfxElMap[key].addEventListener('error', () => {
      update({ sfxAvailable: { ...snapshot.value.sfxAvailable, [key]: false } })
    })
  }

  let gameInProgress = false

  function update(partial: Partial<AudioSnapshot>): void {
    snapshot.value = { ...snapshot.value, ...partial }
  }

  function applyBgmState(): void {
    const s = snapshot.value
    if (!s.bgmAvailable) return
    if (s.bgmSwitch === 'On' && s.unlockState === 'Unlocked' && gameInProgress) {
      bgmEl.volume = s.volume / 100
      bgmEl.play().catch(() => {})
      update({ bgmPlayState: 'Playing' })
    } else {
      bgmEl.pause()
      update({ bgmPlayState: 'Paused' })
    }
  }

  function unlock(): void {
    if (snapshot.value.unlockState === 'Unlocked') return
    bgmEl.play().catch(() => {}).then(() => {
      bgmEl.pause()
      bgmEl.currentTime = 0
    })
    update({ unlockState: 'Unlocked' })
    applyBgmState()
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
    applyBgmState()
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