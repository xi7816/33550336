export function randomInt(min: number, max: number): number {
  if (min > max) {
    throw new RangeError(`randomInt: min(${min}) > max(${max})`)
  }
  const range = max - min + 1
  const maxUint32 = 0xFFFFFFFF
  const limit = maxUint32 - (maxUint32 % range)
  const buf = new Uint32Array(1)
  let value: number
  do {
    crypto.getRandomValues(buf)
    value = buf[0]
  } while (value > limit)
  return min + (value % range)
}

export function randomPick<T>(items: readonly T[]): T {
  if (items.length === 0) {
    throw new RangeError('随机选取集合不能为空')
  }
  return items[randomInt(0, items.length - 1)]
}