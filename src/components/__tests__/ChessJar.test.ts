import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import ChessJar from '../ChessJar.vue'

describe('ChessJar', () => {
  it('owner=Black 渲染"黑"字标签且 data-owner=Black', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: false, neutral: false } })
    expect(w.find('.jar-label').text()).toBe('黑')
    expect(w.find('.chess-jar').attributes('data-owner')).toBe('Black')
  })

  it('owner=White 渲染"白"字标签且 data-owner=White', () => {
    const w = mount(ChessJar, { props: { owner: 'White', active: false, neutral: false } })
    expect(w.find('.jar-label').text()).toBe('白')
    expect(w.find('.chess-jar').attributes('data-owner')).toBe('White')
  })

  it('active=true 时根节点含 is-active 类', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: true, neutral: false } })
    expect(w.find('.chess-jar').classes()).toContain('is-active')
  })

  it('active=false 时根节点不含 is-active 类', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: false, neutral: false } })
    expect(w.find('.chess-jar').classes()).not.toContain('is-active')
  })

  it('neutral=true 时根节点含 is-neutral 类', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: false, neutral: true } })
    expect(w.find('.chess-jar').classes()).toContain('is-neutral')
  })

  it('neutral=false 时根节点不含 is-neutral 类', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: false, neutral: false } })
    expect(w.find('.chess-jar').classes()).not.toContain('is-neutral')
  })

  it('默认静默态仅含 .chess-jar 基础类', () => {
    const w = mount(ChessJar, { props: { owner: 'Black', active: false, neutral: false } })
    const classes = w.find('.chess-jar').classes()
    expect(classes).toContain('chess-jar')
    expect(classes).not.toContain('is-active')
    expect(classes).not.toContain('is-neutral')
  })
})