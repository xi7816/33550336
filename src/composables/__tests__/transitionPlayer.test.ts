import { describe, it, expect, vi, beforeEach } from 'vitest'
import { createTransitionPlayer, TRANSITION_URL } from '../transitionPlayer'

describe('transitionPlayer', () => {
  it('TRANSITION_URL 为固定常量', () => {
    expect(TRANSITION_URL).toBe('/transition/gg.mp4')
  })

  it('createTransitionPlayer 返回完整接口', () => {
    const player = createTransitionPlayer()
    expect(typeof player.play).toBe('function')
    expect(typeof player.onEnded).toBe('function')
    expect(typeof player.onError).toBe('function')
    expect(typeof player.stop).toBe('function')
    expect(typeof player.dispose).toBe('function')
    expect(typeof player.getDuration).toBe('function')
    expect(typeof player.setVolume).toBe('function')
  })

  it('play 设置 src 和 volume', () => {
    const player = createTransitionPlayer()
    expect(() => player.play('/test.mp4', 50)).not.toThrow()
  })

  it('setVolume 不抛异常', () => {
    const player = createTransitionPlayer()
    expect(() => player.setVolume(80)).not.toThrow()
    expect(() => player.setVolume(0)).not.toThrow()
    expect(() => player.setVolume(100)).not.toThrow()
  })

  it('stop 和 dispose 不抛异常', () => {
    const player = createTransitionPlayer()
    player.play('/test.mp4', 50)
    expect(() => player.stop()).not.toThrow()
    expect(() => player.dispose()).not.toThrow()
  })

  it('onEnded 和 onError 注册回调不抛异常', () => {
    const player = createTransitionPlayer()
    const endedCb = vi.fn()
    const errorCb = vi.fn()
    expect(() => player.onEnded(endedCb)).not.toThrow()
    expect(() => player.onError(errorCb)).not.toThrow()
  })
})