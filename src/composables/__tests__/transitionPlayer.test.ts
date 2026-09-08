import { describe, it, expect, vi } from 'vitest'
import { createTransitionPlayer, TRANSITION_URL } from '../transitionPlayer'

describe('transitionPlayer', () => {
  it('TRANSITION_URL 为固定常量', () => {
    expect(TRANSITION_URL).toBe('/transition/gg.mp4')
  })

  it('createTransitionPlayer 返回完整接口（含 attach/detach）', () => {
    const player = createTransitionPlayer()
    expect(typeof player.play).toBe('function')
    expect(typeof player.onEnded).toBe('function')
    expect(typeof player.onError).toBe('function')
    expect(typeof player.stop).toBe('function')
    expect(typeof player.dispose).toBe('function')
    expect(typeof player.getDuration).toBe('function')
    expect(typeof player.setVolume).toBe('function')
    expect(typeof player.attach).toBe('function')
    expect(typeof player.detach).toBe('function')
  })

  it('play 设置 src 和 volume 不抛异常', () => {
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

  it('attach 将视频元素挂载到容器', () => {
    const player = createTransitionPlayer()
    const container = document.createElement('div')
    player.attach(container)
    expect(container.children.length).toBeGreaterThan(0)
  })

  it('detach 从容器移除视频元素', () => {
    const player = createTransitionPlayer()
    const container = document.createElement('div')
    player.attach(container)
    expect(container.children.length).toBeGreaterThan(0)
    player.detach()
    expect(container.children.length).toBe(0)
  })

  it('stop 触发 detach 移除视频元素', () => {
    const player = createTransitionPlayer()
    const container = document.createElement('div')
    player.attach(container)
    player.play('/test.mp4', 50)
    player.stop()
    expect(container.children.length).toBe(0)
  })
})
