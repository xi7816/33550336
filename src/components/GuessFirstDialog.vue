<script setup lang="ts">
import type { PlayerSeat, Parity } from '@/domain/types'
import type { OpeningSnapshot } from '@/composables/useOpening'

const props = defineProps<{
  visible: boolean
  snapshot: OpeningSnapshot
}>()

const emit = defineEmits<{
  rollNumber: []
  guess: [parity: Parity]
}>()

function getOneSeat(): PlayerSeat {
  if (!props.snapshot.assignment) return 'A'
  return props.snapshot.assignment.seatAssign === 'AOneBTwo' ? 'A' : 'B'
}

function getTwoSeat(): PlayerSeat {
  if (!props.snapshot.assignment) return 'B'
  return props.snapshot.assignment.seatAssign === 'AOneBTwo' ? 'B' : 'A'
}

const oneSeat = getOneSeat()
const twoSeat = getTwoSeat()

function playerLabel(player: 'Black' | 'White' | null): string {
  if (!player) return ''
  return player === 'Black' ? '黑方' : '白方'
}
</script>

<template>
  <el-dialog
    :model-value="visible"
    title="猜先手"
    width="90%"
    style="max-width: 480px"
    :close-on-click-modal="false"
    :close-on-press-escape="false"
    :show-close="false"
    align-center
  >
    <div class="guess-content">
      <p class="hint">一号出题（随机生成一个数），二号猜单双，猜对二号先手，猜错一号先手</p>

      <div class="seat-info">
        <span>一号：席位 {{ oneSeat }}</span>
        <span>二号：席位 {{ twoSeat }}</span>
      </div>

      <template v-if="snapshot.guessFirstState === 'WaitingRoll'">
        <div class="action-area">
          <el-button type="primary" @click="emit('rollNumber')">
            一号点击出题
          </el-button>
          <div class="disabled-hint">
            <el-button disabled>单数</el-button>
            <el-button disabled>双数</el-button>
            <p class="wait-text">请等待一号出题</p>
          </div>
        </div>
      </template>

      <template v-if="snapshot.guessFirstState === 'Rolled'">
        <div class="action-area">
          <el-button type="primary" disabled>已出题，请等待二号猜测</el-button>
          <div class="guess-buttons">
            <p class="two-hint">二号请猜：</p>
            <el-button type="warning" @click="emit('guess', 'Odd')">单数</el-button>
            <el-button type="warning" @click="emit('guess', 'Even')">双数</el-button>
          </div>
        </div>
      </template>

      <template v-if="snapshot.guessFirstState === 'Judged'">
        <div class="result-area">
          <p class="revealed">一号所出数：<strong>{{ snapshot.revealedNumber }}</strong>（{{ snapshot.revealedParity === 'Odd' ? '单数' : '双数' }}）</p>
          <p class="first-player">先手方：{{ playerLabel(snapshot.firstPlayer) }}</p>
          <p class="second-player">后手方：{{ playerLabel(snapshot.secondPlayer) }}</p>
          <p class="next-hint">即将进入对局...</p>
        </div>
      </template>
    </div>
  </el-dialog>
</template>

<style scoped>
.guess-content {
  text-align: center;
}

.hint {
  color: #909399;
  margin-bottom: 20px;
}

.seat-info {
  display: flex;
  justify-content: center;
  gap: 24px;
  margin-bottom: 24px;
  font-weight: 600;
}

.action-area {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 20px;
}

.disabled-hint {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 8px;
}

.wait-text {
  color: #909399;
  font-size: 14px;
}

.guess-buttons {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 8px;
}

.two-hint {
  color: #e6a23c;
  font-weight: 600;
}

.result-area {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
}

.revealed {
  font-size: 18px;
}

.first-player {
  font-size: 20px;
  color: #e74c3c;
  font-weight: 700;
}

.second-player {
  font-size: 16px;
  color: #909399;
}

.next-hint {
  margin-top: 12px;
  color: #909399;
}
</style>