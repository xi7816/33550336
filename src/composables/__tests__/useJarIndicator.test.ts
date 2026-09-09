import { describe, it, expect } from 'vitest'
import { shallowRef, computed } from 'vue'
import { deriveJarState, useJarIndicator } from '../useJarIndicator'
import type { Player, GameState } from '@/domain/types'

describe('deriveJarState', () => {
  it('InProgress + Black → 黑激活', () => {
    const s = deriveJarState('Black', 'InProgress')
    expect(s.blackActive).toBe(true)
    expect(s.whiteActive).toBe(false)
    expect(s.neutral).toBe(false)
  })

  it('InProgress + White → 白激活', () => {
    const s = deriveJarState('White', 'InProgress')
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(true)
    expect(s.neutral).toBe(false)
  })

  it('BlackWin → 黑激活锁定（不随 current 变化）', () => {
    const s1 = deriveJarState('Black', 'BlackWin')
    expect(s1.blackActive).toBe(true)
    expect(s1.whiteActive).toBe(false)
    const s2 = deriveJarState('White', 'BlackWin')
    expect(s2.blackActive).toBe(true)
    expect(s2.whiteActive).toBe(false)
  })

  it('WhiteWin → 白激活锁定', () => {
    const s = deriveJarState('Black', 'WhiteWin')
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(true)
  })

  it('Draw → 两罐静默 neutral=true', () => {
    const s = deriveJarState('Black', 'Draw')
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(false)
    expect(s.neutral).toBe(true)
  })

  it('Uninitialized → 两罐静默 neutral=false', () => {
    const s = deriveJarState('Black', 'Uninitialized')
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(false)
    expect(s.neutral).toBe(false)
  })

  it('非预期 gameState 走 default 回退静默不抛异常', () => {
    const s = deriveJarState('Black', 'Unknown' as GameState)
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(false)
    expect(s.neutral).toBe(false)
  })

  it('不变量: 非 Draw 时 blackActive XOR whiteActive === true', () => {
    const cases: Array<[Player, GameState]> = [
      ['Black', 'InProgress'],
      ['White', 'InProgress'],
      ['Black', 'BlackWin'],
      ['White', 'BlackWin'],
      ['Black', 'WhiteWin'],
      ['White', 'WhiteWin']
    ]
    for (const [p, g] of cases) {
      const s = deriveJarState(p, g)
      expect(s.blackActive !== s.whiteActive).toBe(true)
    }
  })

  it('不变量: neutral=true 时双罐均 false', () => {
    const s = deriveJarState('Black', 'Draw')
    expect(s.neutral).toBe(true)
    expect(s.blackActive).toBe(false)
    expect(s.whiteActive).toBe(false)
  })
})

describe('useJarIndicator', () => {
  it('响应式订阅: 修改源值自动重算', () => {
    const player = shallowRef<Player>('Black')
    const state = shallowRef<GameState>('InProgress')
    const { blackActive, whiteActive } = useJarIndicator(() => player.value, () => state.value)
    expect(blackActive.value).toBe(true)
    expect(whiteActive.value).toBe(false)
    player.value = 'White'
    expect(blackActive.value).toBe(false)
    expect(whiteActive.value).toBe(true)
  })

  it('支持 ComputedRef 入参形式', () => {
    const player = computed(() => 'Black' as Player)
    const state = computed(() => 'InProgress' as GameState)
    const { blackActive } = useJarIndicator(player, state)
    expect(blackActive.value).toBe(true)
  })

  it('Draw 态下 blackActive=false whiteActive=false neutral=true', () => {
    const { blackActive, whiteActive, neutral } = useJarIndicator(
      () => 'Black' as Player,
      () => 'Draw' as GameState
    )
    expect(blackActive.value).toBe(false)
    expect(whiteActive.value).toBe(false)
    expect(neutral.value).toBe(true)
  })
})