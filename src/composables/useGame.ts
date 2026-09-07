import { shallowRef, type ShallowRef } from 'vue'
import { Board } from '@/domain/Board'
import { checkWin, checkDraw } from '@/domain/Judge'
import type { GameState, Player, Position, WinningLine, MoveRecord } from '@/domain/types'
import { showError, showResult } from '@/utils/message'

export interface GameSnapshot {
  board: Board | null
  currentPlayer: Player
  gameState: GameState
  history: MoveRecord[]
  winningLine: WinningLine | null
  boardSize: number
}

export interface UseGame {
  snapshot: ShallowRef<GameSnapshot>
  initGame: (size: number) => void
  placeStone: (pos: Position) => void
  undo: () => void
  restart: () => void
}

function createInitialSnapshot(): GameSnapshot {
  return {
    board: null,
    currentPlayer: 'Black',
    gameState: 'Uninitialized',
    history: [],
    winningLine: null,
    boardSize: 15
  }
}

function opponent(p: Player): Player {
  return p === 'Black' ? 'White' : 'Black'
}

const DIR_NAMES: Record<string, string> = {
  Horizontal: '横向',
  Vertical: '纵向',
  LeftDiagonal: '左斜',
  RightDiagonal: '右斜'
}

export function useGame(): UseGame {
  const snapshot = shallowRef<GameSnapshot>(createInitialSnapshot())

  function update(partial: Partial<GameSnapshot>): void {
    snapshot.value = { ...snapshot.value, ...partial }
  }

  function initGame(size: number): void {
    try {
      const board = new Board(size)
      update({
        board,
        currentPlayer: 'Black',
        gameState: 'InProgress',
        history: [],
        winningLine: null,
        boardSize: size
      })
    } catch {
      showError('棋盘尺寸须为 5 至 19 之间的奇数')
    }
  }

  function placeStone(pos: Position): void {
    const s = snapshot.value
    if (s.gameState !== 'InProgress' || !s.board) return

    if (!s.board.isValidPosition(pos)) {
      showError('坐标越界')
      return
    }
    if (!s.board.isEmpty(pos)) {
      showError('该位置已有棋子，请选择其他位置')
      return
    }

    s.board.placeStone(pos, s.currentPlayer)
    const newBoard = s.board.clone()
    const history = [...s.history, { pos, player: s.currentPlayer }]

    const win = checkWin(s.board, pos, s.currentPlayer)
    if (win) {
      const state: GameState = s.currentPlayer === 'Black' ? 'BlackWin' : 'WhiteWin'
      update({ board: newBoard, history, gameState: state, winningLine: win })
      const winner = s.currentPlayer === 'Black' ? '黑方' : '白方'
      showResult(
        `${winner}获胜！`,
        `连珠方向：${DIR_NAMES[win.direction]}，起点 (${win.start.row}, ${win.start.col})，长度 ${win.length}`
      )
      return
    }

    if (checkDraw(s.board)) {
      update({ board: newBoard, history, gameState: 'Draw' })
      showResult('平局', '棋盘已满，双方未形成五子连珠')
      return
    }

    update({ board: newBoard, history, currentPlayer: opponent(s.currentPlayer) })
  }

  function undo(): void {
    const s = snapshot.value
    if (s.gameState !== 'InProgress') {
      showError('对局已结束，如需继续请开始新局')
      return
    }
    if (s.history.length === 0) {
      showError('当前无步可悔')
      return
    }

    const history = [...s.history]
    const last = history.pop()!
    s.board!.clearStone(last.pos)
    const newBoard = s.board!.clone()
    update({ board: newBoard, history, currentPlayer: last.player, winningLine: null })
  }

  function restart(): void {
    update(createInitialSnapshot())
  }

  return {
    snapshot,
    initGame,
    placeStone,
    undo,
    restart
  }
}