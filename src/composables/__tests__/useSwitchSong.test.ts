import { describe, it, expect, vi } from 'vitest'
import { useSwitchSong } from '../useSwitchSong'
import type { UseAudio, AudioSnapshot } from '../useAudio'
import type { ShallowRef } from 'vue'
import { shallowRef } from 'vue'

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
  it('初始状态为 Idle 且覆盖层不可见', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(ss.snapshot.value.lastResult).toBe('Success')
    expect(ss.overlayVisible.value).toBe(false)
  })

  it('Locked 状态下点击不响应', () => {
    const audio = createMockAudio({ unlockState: 'Locked' })
    const ss = useSwitchSong(audio)
    ss.handleClick()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(audio.pauseBgmPlayback).not.toHaveBeenCalled()
  })

  it('点击后进入 Transitioning、暂停 BGM、显示覆盖层', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    expect(ss.snapshot.value.state).toBe('Transitioning')
    expect(audio.pauseBgmPlayback).toHaveBeenCalledTimes(1)
    expect(ss.overlayVisible.value).toBe(true)
  })

  it('Transitioning 状态下再次点击被忽略', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleClick()
    expect(audio.pauseBgmPlayback).toHaveBeenCalledTimes(1)
  })

  it('handleContainerReady 是函数', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(typeof ss.handleContainerReady).toBe('function')
  })

  it('handleContainerReady 不抛异常', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    const el = document.createElement('div')
    expect(() => ss.handleContainerReady(el)).not.toThrow()
  })

  it('overlayVisible 是 ref', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(typeof ss.overlayVisible.value).toBe('boolean')
  })

  it('点击后 skipButtonVisible 为 true', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    expect(ss.skipButtonVisible.value).toBe(true)
  })

  it('UT-01: 过渡期间 handleSkip 后状态回 Idle、覆盖层与跳过按钮隐藏、切下一曲', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(ss.overlayVisible.value).toBe(false)
    expect(ss.skipButtonVisible.value).toBe(false)
    expect(audio.switchToNextTrack).toHaveBeenCalledTimes(1)
  })

  it('UT-02: Idle 状态下 handleSkip 静默忽略', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleSkip()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(audio.switchToNextTrack).not.toHaveBeenCalled()
  })

  it('UT-03: Transitioning 下连续两次 handleSkip 仅首次生效', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    ss.handleSkip()
    expect(audio.switchToNextTrack).toHaveBeenCalledTimes(1)
  })

  it('UT-04: handleSkip 与自然结束并发仅切一次下一曲', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    ss.handleSkip()
    ss.handleSkip()
    expect(audio.switchToNextTrack).toHaveBeenCalledTimes(1)
    expect(ss.snapshot.value.state).toBe('Idle')
  })

  it('UT-05: bgmSwitch Off 时 handleSkip 停止但不切下一曲', () => {
    const audio = createMockAudio({ bgmSwitch: 'Off' })
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(audio.switchToNextTrack).not.toHaveBeenCalled()
  })

  it('UT-06: bgmAvailable false 时 handleSkip 停止但不切下一曲', () => {
    const audio = createMockAudio({ bgmAvailable: false })
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    expect(ss.snapshot.value.state).toBe('Idle')
    expect(audio.switchToNextTrack).not.toHaveBeenCalled()
  })

  it('handleSkip 后 skipButtonVisible 为 false', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    ss.handleClick()
    ss.handleSkip()
    expect(ss.skipButtonVisible.value).toBe(false)
  })

  it('handleSkip 是函数', () => {
    const audio = createMockAudio()
    const ss = useSwitchSong(audio)
    expect(typeof ss.handleSkip).toBe('function')
  })
})
