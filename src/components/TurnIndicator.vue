<script setup lang="ts">
import type { Player, GameState } from '@/domain/types'

defineProps<{
  currentPlayer: Player
  gameState: GameState
}>()
</script>

<template>
  <div class="turn-indicator">
    <template v-if="gameState === 'Uninitialized'">
      <span class="hint">请选择棋盘尺寸并开始新局</span>
    </template>
    <template v-else-if="gameState === 'InProgress'">
      <span class="label">当前回合：</span>
      <span
        class="player-tag"
        :class="currentPlayer === 'Black' ? 'black' : 'white'"
      >
        {{ currentPlayer === 'Black' ? '黑方' : '白方' }}
      </span>
    </template>
    <template v-else>
      <span class="result" :class="gameState">
        {{ gameState === 'BlackWin' ? '黑方获胜' : gameState === 'WhiteWin' ? '白方获胜' : '平局' }}
      </span>
    </template>
  </div>
</template>

<style scoped>
.turn-indicator {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 12px 20px;
  font-size: 18px;
  font-weight: 600;
}

.hint {
  color: #909399;
}

.label {
  color: #606266;
}

.player-tag {
  padding: 4px 16px;
  border-radius: 6px;
  font-size: 16px;
}

.player-tag.black {
  background-color: #1a1a1a;
  color: #fff;
}

.player-tag.white {
  background-color: #f5f5f5;
  color: #1a1a1a;
  border: 1px solid #dcdfe6;
}

.result {
  padding: 4px 16px;
  border-radius: 6px;
  background-color: #e74c3c;
  color: #fff;
}
</style>