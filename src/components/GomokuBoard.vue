<script setup lang="ts">
import { computed } from 'vue'
import type { Board } from '@/domain/Board'
import type { Position, WinningLine, Direction } from '@/domain/types'

const props = defineProps<{
  board: Board | null
  winningLine: WinningLine | null
  disabled: boolean
}>()

const emit = defineEmits<{
  place: [pos: Position]
}>()

const size = computed(() => props.board?.size ?? 15)
const total = computed(() => size.value + 1)
const viewBox = computed(() => `0 0 ${total.value} ${total.value}`)

interface StoneDisplay {
  row: number
  col: number
  fill: string
  stroke: string
}

const stones = computed<StoneDisplay[]>(() => {
  if (!props.board) return []
  const result: StoneDisplay[] = []
  for (let r = 1; r <= size.value; r++) {
    for (let c = 1; c <= size.value; c++) {
      const cell = props.board.getCell({ row: r, col: c })
      if (cell === 'Black') {
        result.push({ row: r, col: c, fill: '#1a1a1a', stroke: 'none' })
      } else if (cell === 'White') {
        result.push({ row: r, col: c, fill: '#f5f5f5', stroke: '#999' })
      }
    }
  }
  return result
})

const DIR_VECTORS: Record<Direction, { dr: number; dc: number }> = {
  Horizontal: { dr: 0, dc: 1 },
  Vertical: { dr: 1, dc: 0 },
  LeftDiagonal: { dr: 1, dc: 1 },
  RightDiagonal: { dr: 1, dc: -1 }
}

const highlights = computed<Position[]>(() => {
  if (!props.winningLine) return []
  const { direction, start, length } = props.winningLine
  const { dr, dc } = DIR_VECTORS[direction]
  const result: Position[] = []
  for (let i = 0; i < length; i++) {
    result.push({
      row: start.row + dr * i,
      col: start.col + dc * i
    })
  }
  return result
})

function handleSvgClick(event: MouseEvent): void {
  if (props.disabled || !props.board) return
  const svg = event.currentTarget as SVGSVGElement
  const rect = svg.getBoundingClientRect()
  const x = ((event.clientX - rect.left) / rect.width) * total.value
  const y = ((event.clientY - rect.top) / rect.height) * total.value
  const col = Math.round(x)
  const row = Math.round(y)
  if (row >= 1 && row <= size.value && col >= 1 && col <= size.value) {
    emit('place', { row, col })
  }
}

function handleTouch(event: TouchEvent): void {
  if (props.disabled || !props.board) return
  event.preventDefault()
  const touch = event.touches[0]
  if (!touch) return
  const svg = event.currentTarget as SVGSVGElement
  const rect = svg.getBoundingClientRect()
  const x = ((touch.clientX - rect.left) / rect.width) * total.value
  const y = ((touch.clientY - rect.top) / rect.height) * total.value
  const col = Math.round(x)
  const row = Math.round(y)
  if (row >= 1 && row <= size.value && col >= 1 && col <= size.value) {
    emit('place', { row, col })
  }
}
</script>

<template>
  <div class="board-wrapper">
    <svg
      :viewBox="viewBox"
      class="board-svg"
      @click="handleSvgClick"
      @touchstart="handleTouch"
    >
      <rect :x="0.5" :y="0.5" :width="size" :height="size" fill="#e8d8b8" rx="0.1" />

      <line
        v-for="r in size"
        :key="'h' + r"
        :x1="1"
        :y1="r"
        :x2="size"
        :y2="r"
        stroke="#8b7355"
        stroke-width="0.025"
      />
      <line
        v-for="c in size"
        :key="'v' + c"
        :x1="c"
        :y1="1"
        :x2="c"
        :y2="size"
        stroke="#8b7355"
        stroke-width="0.025"
      />

      <text
        v-for="c in size"
        :key="'col' + c"
        :x="c"
        :y="0.7"
        text-anchor="middle"
        font-size="0.28"
        fill="#8b7355"
      >{{ c }}</text>
      <text
        v-for="r in size"
        :key="'row' + r"
        :x="0.6"
        :y="r + 0.1"
        text-anchor="middle"
        font-size="0.28"
        fill="#8b7355"
      >{{ r }}</text>

      <circle
        v-for="(s, i) in stones"
        :key="'s' + i"
        :cx="s.col"
        :cy="s.row"
        :r="0.42"
        :fill="s.fill"
        :stroke="s.stroke"
        stroke-width="0.05"
      />

      <circle
        v-for="(hl, i) in highlights"
        :key="'hl' + i"
        :cx="hl.col"
        :cy="hl.row"
        :r="0.46"
        fill="none"
        stroke="#e74c3c"
        stroke-width="0.08"
      />
    </svg>
  </div>
</template>

<style scoped>
.board-wrapper {
  display: flex;
  justify-content: center;
  align-items: center;
  flex: 1 1 auto;
  padding: 20px;
  width: 100%;
  min-height: 0;
}

.board-svg {
  width: min(85vw, 75vh);
  height: min(85vw, 75vh);
  cursor: pointer;
  filter: drop-shadow(0 4px 12px rgba(0, 0, 0, 0.2));
}
</style>