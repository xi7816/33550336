<script setup lang="ts">
import { ref, watch } from 'vue'
import type { GameState } from '@/domain/types'

const props = defineProps<{
  gameState: GameState
  historyLength: number
  currentSize: number
}>()

const emit = defineEmits<{
  init: [size: number]
  undo: []
  restart: []
}>()

const sizeOptions = [5, 7, 9, 11, 13, 15, 17, 19]
const selectedSize = ref(15)

watch(
  () => props.currentSize,
  (val) => { selectedSize.value = val }
)

function handleInit(): void {
  emit('init', selectedSize.value)
}
</script>

<template>
  <div class="control-bar">
    <el-select
      v-model="selectedSize"
      :disabled="gameState !== 'Uninitialized'"
      size="default"
      style="width: 120px"
    >
      <el-option
        v-for="s in sizeOptions"
        :key="s"
        :label="`${s} × ${s}`"
        :value="s"
      />
    </el-select>

    <el-button
      type="primary"
      :disabled="gameState === 'InProgress'"
      @click="handleInit"
    >
      {{ gameState === 'Uninitialized' ? '开始新局' : '重新开始' }}
    </el-button>

    <el-button
      :disabled="historyLength === 0 || gameState !== 'InProgress'"
      @click="emit('undo')"
    >
      悔棋
    </el-button>

    <el-button
      type="warning"
      :disabled="gameState === 'Uninitialized' || gameState === 'InProgress'"
      @click="emit('restart')"
    >
      重开
    </el-button>
  </div>
</template>

<style scoped>
.control-bar {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 12px;
  padding: 16px 20px;
}
</style>