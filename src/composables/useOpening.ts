import { shallowRef, type ShallowRef } from 'vue'
import { randomInt, randomPick } from '@/utils/random'
import type {
  OpeningPhase,
  ColorSelectState,
  GuessFirstState,
  OpeningAssignment,
  PlayerSeat,
  Parity,
  Player,
  ColorAssignResult,
  SeatAssignResult
} from '@/domain/types'
import { showError } from '@/utils/message'

export interface OpeningSnapshot {
  phase: OpeningPhase
  colorSelectState: ColorSelectState
  confirmedA: boolean
  confirmedB: boolean
  boardSize: number
  assignment: OpeningAssignment | null
  guessFirstState: GuessFirstState
  revealedNumber: number | null
  revealedParity: Parity | null
  firstPlayer: Player | null
  secondPlayer: Player | null
}

export interface UseOpening {
  snapshot: ShallowRef<OpeningSnapshot>
  confirm: (seat: PlayerSeat) => void
  selectSize: (size: number) => void
  rollNumber: () => void
  guess: (parity: Parity) => void
  reset: () => void
  onCompleted: (cb: (size: number, firstPlayer: Player) => void) => void
}

function createInitialSnapshot(): OpeningSnapshot {
  return {
    phase: 'ColorSelect',
    colorSelectState: 'Pending',
    confirmedA: false,
    confirmedB: false,
    boardSize: 15,
    assignment: null,
    guessFirstState: 'WaitingRoll',
    revealedNumber: null,
    revealedParity: null,
    firstPlayer: null,
    secondPlayer: null
  }
}

function getSeatPlayer(assignment: OpeningAssignment, seat: PlayerSeat): Player {
  const aIsBlack = assignment.colorAssign === 'ABlackBWhite'
  if (seat === 'A') return aIsBlack ? 'Black' : 'White'
  return aIsBlack ? 'White' : 'Black'
}

function getOneSeat(assignment: OpeningAssignment): PlayerSeat {
  return assignment.seatAssign === 'AOneBTwo' ? 'A' : 'B'
}

function getTwoSeat(assignment: OpeningAssignment): PlayerSeat {
  return assignment.seatAssign === 'AOneBTwo' ? 'B' : 'A'
}

export function useOpening(): UseOpening {
  const snapshot = shallowRef<OpeningSnapshot>(createInitialSnapshot())
  let secretNumber = 0
  let onCompletedCb: ((size: number, firstPlayer: Player) => void) | null = null

  function update(partial: Partial<OpeningSnapshot>): void {
    snapshot.value = { ...snapshot.value, ...partial }
  }

  function confirm(seat: PlayerSeat): void {
    const s = snapshot.value
    if (s.phase !== 'ColorSelect') return

    if (seat === 'A') {
      if (s.confirmedA) {
        showError('已确认，等待对方')
        return
      }
      update({ confirmedA: true })
    } else {
      if (s.confirmedB) {
        showError('已确认，等待对方')
        return
      }
      update({ confirmedB: true })
    }

    const ns = snapshot.value
    if (ns.confirmedA && ns.confirmedB) {
      const colorAssign = randomPick<ColorAssignResult>(['ABlackBWhite', 'AWhiteBBlack'])
      const seatAssign = randomPick<SeatAssignResult>(['AOneBTwo', 'ATwoBOne'])
      update({
        colorSelectState: 'Assigned',
        assignment: { colorAssign, seatAssign },
        phase: 'GuessFirst',
        guessFirstState: 'WaitingRoll'
      })
    }
  }

  function selectSize(size: number): void {
    if (size < 5 || size > 19 || size % 2 === 0) {
      showError('棋盘尺寸须为 5 至 19 之间的奇数，请重新选择')
      return
    }
    update({ boardSize: size })
  }

  function rollNumber(): void {
    const s = snapshot.value
    if (s.phase !== 'GuessFirst') return
    if (s.guessFirstState !== 'WaitingRoll') {
      showError('已出题，请等待2号猜测')
      return
    }
    secretNumber = randomInt(1, 100)
    update({ guessFirstState: 'Rolled' })
  }

  function guess(parity: Parity): void {
    const s = snapshot.value
    if (s.phase !== 'GuessFirst') return
    if (s.guessFirstState !== 'Rolled') {
      showError('请等待一号出题')
      return
    }
    if (!s.assignment) return

    const actualParity: Parity = secretNumber % 2 === 0 ? 'Even' : 'Odd'
    const guessedCorrectly = parity === actualParity
    const assignment = s.assignment
    const firstSeat = guessedCorrectly ? getTwoSeat(assignment) : getOneSeat(assignment)
    const secondSeat = guessedCorrectly ? getOneSeat(assignment) : getTwoSeat(assignment)
    const firstPlayer = getSeatPlayer(assignment, firstSeat)
    const secondPlayer = getSeatPlayer(assignment, secondSeat)

    update({
      guessFirstState: 'Judged',
      revealedNumber: secretNumber,
      revealedParity: actualParity,
      firstPlayer,
      secondPlayer,
      phase: 'Completed'
    })

    if (onCompletedCb) {
      onCompletedCb(s.boardSize, firstPlayer)
    }
  }

  function reset(): void {
    secretNumber = 0
    update(createInitialSnapshot())
  }

  function onCompleted(cb: (size: number, firstPlayer: Player) => void): void {
    onCompletedCb = cb
  }

  return {
    snapshot,
    confirm,
    selectSize,
    rollNumber,
    guess,
    reset,
    onCompleted
  }
}