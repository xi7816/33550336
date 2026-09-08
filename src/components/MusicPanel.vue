<script setup lang="ts">
import type { AudioSnapshot } from '@/composables/useAudio'

const props = defineProps<{
  snapshot: AudioSnapshot
}>()

const emit = defineEmits<{
  toggleBgm: []
  setVolume: [value: number]
  toggleSfx: []
}>()

function isBgmOn(): boolean {
  return props.snapshot.bgmSwitch === 'On'
}

function isSfxOn(): boolean {
  return props.snapshot.sfxSwitch === 'On'
}

function handleVolumeChange(val: number | number[]): void {
  emit('setVolume', Array.isArray(val) ? val[0] : val)
}
</script>

<template>
  <div class="music-panel">
    <div class="panel-title">古风音乐</div>

    <div class="control-row">
      <span class="label">背景</span>
      <el-switch
        :model-value="isBgmOn()"
        @change="emit('toggleBgm')"
        size="small"
      />
    </div>

    <div class="control-row">
      <span class="label">音效</span>
      <el-switch
        :model-value="isSfxOn()"
        @change="emit('toggleSfx')"
        size="small"
      />
    </div>

    <div class="control-row volume-row">
      <span class="label">音量</span>
      <el-slider
        :model-value="snapshot.volume"
        :min="0"
        :max="100"
        :step="1"
        :show-tooltip="false"
        size="small"
        style="flex: 1; margin-left: 8px"
        @change="handleVolumeChange"
      />
    </div>

    <p v-if="snapshot.unlockState === 'Locked'" class="unlock-hint">
      点击页面任意位置开启声音
    </p>
  </div>
</template>

<style scoped>
.music-panel {
  position: fixed;
  top: 16px;
  right: 16px;
  z-index: 100;
  padding: 12px 16px;
  background: rgba(232, 216, 184, 0.92);
  border: 2px solid #8b7355;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
  min-width: 180px;
}

.panel-title {
  font-size: 14px;
  font-weight: 600;
  color: #5a4a32;
  text-align: center;
  margin-bottom: 10px;
  border-bottom: 1px solid #c4a97d;
  padding-bottom: 6px;
}

.control-row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  margin-bottom: 8px;
}

.volume-row {
  margin-bottom: 4px;
}

.label {
  font-size: 13px;
  color: #5a4a32;
  white-space: nowrap;
}

.unlock-hint {
  font-size: 12px;
  color: #e6a23c;
  text-align: center;
  margin-top: 6px;
  margin-bottom: 0;
  animation: blink 1.5s ease-in-out infinite;
}

@keyframes blink {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.4; }
}
</style>