<!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>Списки</title>
  <style>
   li {
    list-style: none; /* Скрываем маркеры списка */
    text-indent: -1em; /* Сдвигаем элементы списка влево */
   }
   li::before {
    content: '\25ba'; /* Добавляем маркер в шестнадцатеричном формате*/
    padding-right: 5px; /* Расстояние от маркеров до текста */
    color: red; /* Маркеры красного цвета */
   }
  </style>
 </head>
 <body>
  <ul>
   <li>Сепульки</li>
   <li>Сепулькарии</li>
   <li>Сепуление</li>
  </ul>
 </body>
</html>

