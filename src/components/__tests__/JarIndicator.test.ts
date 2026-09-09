import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import JarIndicator from '../JarIndicator.vue'

describe('JarIndicator', () => {
  it('visible=false 时不渲染任何节点', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'InProgress', visible: false }
    })
    expect(w.find('.jar-indicator').exists()).toBe(false)
  })

  it('visible=true 时渲染 .jar-indicator 容器且含恰好两个 .chess-jar', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'InProgress', visible: true }
    })
    expect(w.find('.jar-indicator').exists()).toBe(true)
    expect(w.findAll('.chess-jar')).toHaveLength(2)
  })

  it('黑罐在前(左) 白罐在后(右)', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'InProgress', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[0].attributes('data-owner')).toBe('Black')
    expect(jars[1].attributes('data-owner')).toBe('White')
  })

  it('Black+InProgress 黑罐激活 白罐不激活', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'InProgress', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[0].classes()).toContain('is-active')
    expect(jars[1].classes()).not.toContain('is-active')
  })

  it('White+InProgress 白罐激活 黑罐不激活', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'White', gameState: 'InProgress', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[1].classes()).toContain('is-active')
    expect(jars[0].classes()).not.toContain('is-active')
  })

  it('Draw 两罐均 is-neutral 均不含 is-active', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'Draw', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[0].classes()).toContain('is-neutral')
    expect(jars[1].classes()).toContain('is-neutral')
    expect(jars[0].classes()).not.toContain('is-active')
    expect(jars[1].classes()).not.toContain('is-active')
  })

  it('BlackWin 黑罐激活 白罐不激活', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'BlackWin', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[0].classes()).toContain('is-active')
    expect(jars[1].classes()).not.toContain('is-active')
  })

  it('WhiteWin 白罐激活 黑罐不激活', () => {
    const w = mount(JarIndicator, {
      props: { currentPlayer: 'Black', gameState: 'WhiteWin', visible: true }
    })
    const jars = w.findAll('.chess-jar')
    expect(jars[1].classes()).toContain('is-active')
    expect(jars[0].classes()).not.toContain('is-active')
  })
})