import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import TransitionVideoOverlay from '../TransitionVideoOverlay.vue'

describe('TransitionVideoOverlay', () => {
  it('UT-10: visible=true 且 skipButtonVisible=true 时覆盖层与跳过按钮均存在', () => {
    const wrapper = mount(TransitionVideoOverlay, {
      props: { visible: true, skipButtonVisible: true }
    })
    expect(wrapper.find('.transition-overlay').exists()).toBe(true)
    expect(wrapper.find('button.skip-button').exists()).toBe(true)
  })

  it('UT-11: visible=true 且 skipButtonVisible=false 时覆盖层存在、跳过按钮不存在', () => {
    const wrapper = mount(TransitionVideoOverlay, {
      props: { visible: true, skipButtonVisible: false }
    })
    expect(wrapper.find('.transition-overlay').exists()).toBe(true)
    expect(wrapper.find('button.skip-button').exists()).toBe(false)
  })

  it('UT-12: 点击跳过按钮 emit skip 事件一次', async () => {
    const wrapper = mount(TransitionVideoOverlay, {
      props: { visible: true, skipButtonVisible: true }
    })
    const button = wrapper.find('button.skip-button')
    await button.trigger('click')
    expect(wrapper.emitted('skip')).toBeTruthy()
    expect(wrapper.emitted('skip')!.length).toBe(1)
  })

  it('UT-13: 点击覆盖层非跳过按钮区域不 emit skip', async () => {
    const wrapper = mount(TransitionVideoOverlay, {
      props: { visible: true, skipButtonVisible: true }
    })
    const overlay = wrapper.find('.transition-overlay')
    await overlay.trigger('click')
    expect(wrapper.emitted('skip')).toBeFalsy()
  })
})