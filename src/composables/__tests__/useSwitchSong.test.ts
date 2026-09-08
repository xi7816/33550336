import { describe, it, expect, vi } from 'vitest'
import { useSwitchSong } from '../useSwitchSong'
import type { UseAudio, AudioSnapshot } from '../useAudio'
import type { ShallowRef } from 'vue'
import { shallowRef } from 'vue'
import type { SwitchSongSnapshot } from '@/domain/types'

function createMockAudio(overrides: Partial<AudioSnapshot> = {}): UseAudio {
  const snapshot: ShallowRef<AudioSnapshot> = shallowRef<AudioSnapshot>({
    bgmSwitch: 'On',
    volume: 50,
    sfxSwitch: 'On',
    unlockState: 'Unlocked',
    bgmPlayState: 'Playing',
    bgmAvailable: true,
    sfxAvailable: { Place: true, Win: true, Draw: true },
    ...overrides
  })
  return {
    snapshot,
    unlock: vi.fn(),
    toggleBgm: vi.fn(),
    setVolume: vi.fn(),
    toggleSfx: vi.fn(),
    onGameStart: vi.fn(),
    onGameEnd: vi.fn(),
    onGameReset: vi.fn(),
    playSfx: vi.fn(),
    pauseBgmPlayback: vi.fn(() => {
      snapshot.value = { ...snapshot.value, bgmPlayState: 'Paused' }
    }),
    switchToNextTrack: vi.fn()
  }
}

describe('useSwitchSong', () => {
  it('初始状态为 Idle', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(ss.snapshot.value.lastResult).toBe('Success')
  })

  it('Locked 状态下点击不响应', () => {
    const audio = createMockAudio({ unlockState: 'Locked' })
    const ss = useSwitchSong(audio)
    ss.handleClick()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(audio.pauseBgmPlayback).not.toHaveBeenCalled()
  })

  it('点击后进入 Transitioning 并暂停 BGM', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    expect(ss.snapshot.value.state).toBe('Transitioning')
    expect(audio.pauseBgmPlayback).toHaveBeenCalledTimes(1)
  })

  it('Transitioning 状态下再次点击被忽略', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleClick()
    expect(audio.pauseBgmPlayback).toHaveBeenCalledTimes(1)
  })

  it('handleClick 是函数', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(typeof ss.handleClick).toBe('function')
  })
})