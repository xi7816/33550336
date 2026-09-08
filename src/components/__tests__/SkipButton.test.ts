import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import SkipButton from '../SkipButton.vue'

describe('SkipButton', () => {
  it('UT-07: visible=true 时按钮存在且文案为"跳过"', () => {
    const wrapper = mount(SkipButton, { props: { visible: true } })
    const button = wrapper.find('button.skip-button')
    expect(button.exists()).toBe(true)
    expect(button.text()).toBe('跳过')
  })

  it('UT-08: visible=false 时按钮不存在', () => {
    const wrapper = mount(SkipButton, { props: { visible: false } })
    const button = wrapper.find('button.skip-button')
    expect(button.exists()).toBe(false)
  })

  it('UT-09: 点击按钮 emit skip 事件一次', async () => {
    const wrapper = mount(SkipButton, { props: { visible: true } })
    const button = wrapper.find('button.skip-button')
    await button.trigger('click')
    expect(wrapper.emitted('skip')).toBeTruthy()
    expect(wrapper.emitted('skip')!.length).toBe(1)
  })
})