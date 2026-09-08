import { ElMessage, ElMessageBox } from 'element-plus'

export function showError(msg: string): void {
  ElMessage.warning({ message: msg, grouping: true })
}

export function showInfo(msg: string): void {
  ElMessage.info({ message: msg })
}

export function showResult(title: string, content: string): Promise<void> {
  return ElMessageBox.alert(content, title, {
    confirmButtonText: '确定',
    type: 'success'
  }).then(() => undefined)
}