<script setup lang="ts">
defineProps<{
  visible: boolean
}>()

const emit = defineEmits<{
  skip: []
}>()

function handleClick(): void {
  emit('skip')
}
</script>

<template>
  <button
    v-if="visible"
    class="skip-button"
    @click.stop="handleClick"
  >
    跳过
  </button>
</template>

<style scoped>
.skip-button {
  position: absolute;
  top: 16px;
  right: 16px;
  z-index: 210;
  padding: 8px 20px;
  background: rgba(0, 0, 0, 0.55);
  color: #ffffff;
  border: 1px solid rgba(255, 255, 255, 0.4);
  border-radius: 8px;
  font-size: 15px;
  line-height: 1.4;
  cursor: pointer;
  user-select: none;
  transition: background 0.2s ease, border-color 0.2s ease;
}

.skip-button:hover {
  background: rgba(0, 0, 0, 0.75);
  border-color: rgba(255, 255, 255, 0.7);
}
</style>