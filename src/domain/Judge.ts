import type { Board } from './Board'
import type { Direction, Player, Position, WinningLine } from './types'

const DIRECTIONS: { dr: number; dc: number; dir: Direction }[] = [
  { dr: 0, dc: 1, dir: 'Horizontal' },
  { dr: 1, dc: 0, dir: 'Vertical' },
  { dr: 1, dc: 1, dir: 'LeftDiagonal' },
  { dr: 1, dc: -1, dir: 'RightDiagonal' }
]

function playerToCell(player: Player): 'Black' | 'White' {
  return player === 'Black' ? 'Black' : 'White'
}

function countDirection(
  board: Board,
  pos: Position,
  player: Player,
  dr: number,
  dc: number
): number {
  const target = playerToCell(player)
  let count = 0
  let curRow = pos.row + dr
  let curCol = pos.col + dc
  while (board.isValidPosition({ row: curRow, col: curCol }) && board.getCell({ row: curRow, col: curCol }) === target) {
    count++
    curRow += dr
    curCol += dc
  }
  return count
}

export function checkWin(
  board: Board,
  lastMove: Position,
  player: Player
): WinningLine | null {
  for (const { dr, dc, dir } of DIRECTIONS) {
    const forward = countDirection(board, lastMove, player, dr, dc)
    const backward = countDirection(board, lastMove, player, -dr, -dc)
    const total = 1 + forward + backward

    if (total >= 5) {
      return {
        direction: dir,
        start: {
          row: lastMove.row - dr * backward,
          col: lastMove.col - dc * backward
        },
        length: total,
        player
      }
    }
  }
  return null
}

export function checkDraw(board: Board): boolean {
  return board.isFull()
}