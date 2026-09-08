<script setup lang="ts">
import { ref, watch } from 'vue'
import type { PlayerSeat } from '@/domain/types'
import type { OpeningSnapshot } from '@/composables/useOpening'

const props = defineProps<{
  visible: boolean
  snapshot: OpeningSnapshot
}>()

const emit = defineEmits<{
  confirm: [seat: PlayerSeat]
  selectSize: [size: number]
}>()

const sizeOptions = [5, 7, 9, 11, 13, 15, 17, 19]
const selectedSize = ref(15)

watch(
  () => props.snapshot.boardSize,
  (val) => { selectedSize.value = val }
)

function handleSizeChange(val: number): void {
  selectedSize.value = val
  emit('selectSize', val)
}

function seatLabel(seat: PlayerSeat): string {
  return seat === 'A' ? '席位 A' : '席位 B'
}

function getColorForSeat(seat: PlayerSeat): string {
  if (!props.snapshot.assignment) return ''
  const aIsBlack = props.snapshot.assignment.colorAssign === 'ABlackBWhite'
  if (seat === 'A') return aIsBlack ? '黑方' : '白方'
  return aIsBlack ? '白方' : '黑方'
}

function getSeatRole(seat: PlayerSeat): string {
  if (!props.snapshot.assignment) return ''
  const aIsOne = props.snapshot.assignment.seatAssign === 'AOneBTwo'
  if (seat === 'A') return aIsOne ? '一号（出题）' : '二号（猜单双）'
  return aIsOne ? '二号（猜单双）' : '一号（出题）'
}
</script>

<template>
  <el-dialog
    :model-value="visible"
    title="选黑白"
    width="90%"
    style="max-width: 480px"
    :close-on-click-modal="false"
    :close-on-press-escape="false"
    :show-close="false"
    align-center
  >
    <template v-if="snapshot.colorSelectState !== 'Assigned'">
      <p class="hint">请双方确认参与，系统将随机分配黑白方与一号/二号</p>

      <div class="size-select">
        <span>棋盘尺寸：</span>
        <el-select
          :model-value="selectedSize"
          @change="handleSizeChange"
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
      </div>

      <div class="seat-buttons">
        <el-button
          type="primary"
          :disabled="snapshot.confirmedA"
          @click="emit('confirm', 'A')"
        >
          {{ snapshot.confirmedA ? '席位 A 已确认' : `${seatLabel('A')} 确认` }}
        </el-button>
        <el-button
          type="success"
          :disabled="snapshot.confirmedB"
          @click="emit('confirm', 'B')"
        >
          {{ snapshot.confirmedB ? '席位 B 已确认' : `${seatLabel('B')} 确认` }}
        </el-button>
      </div>

      <p v-if="snapshot.confirmedA && !snapshot.confirmedB" class="waiting">席位 A 已确认，等待席位 B...</p>
      <p v-if="snapshot.confirmedB && !snapshot.confirmedA" class="waiting">席位 B 已确认，等待席位 A...</p>
    </template>

    <template v-else>
      <div class="assignment-result">
        <div class="result-row">
          <span class="seat">席位 A：</span>
          <span class="color">你执 {{ getColorForSeat('A') }}</span>
          <span class="role">{{ getSeatRole('A') }}</span>
        </div>
        <div class="result-row">
          <span class="seat">席位 B：</span>
          <span class="color">你执 {{ getColorForSeat('B') }}</span>
          <span class="role">{{ getSeatRole('B') }}</span>
        </div>
        <p class="next-hint">即将进入猜先手环节...</p>
      </div>
    </template>
  </el-dialog>
</template>

<style scoped>
.hint {
  text-align: center;
  color: #909399;
  margin-bottom: 20px;
}

.size-select {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  margin-bottom: 24px;
}

.seat-buttons {
  display: flex;
  justify-content: center;
  gap: 16px;
  margin-bottom: 16px;
}

.waiting {
  text-align: center;
  color: #e6a23c;
}

.assignment-result {
  text-align: center;
}

.result-row {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 12px;
  margin-bottom: 16px;
  font-size: 16px;
}

.seat {
  font-weight: 600;
}

.color {
  color: #409eff;
  font-weight: 600;
}

.role {
  color: #e6a23c;
}

.next-hint {
  margin-top: 20px;
  color: #909399;
}
</style>