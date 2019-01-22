script {
  links = {
    {
      2,
      1,
      7,
      2,
    },
    {
      5,
      1,
      7,
      1,
    },
    {
      5,
      2,
      2,
      1,
    },
    {
      5,
      2,
      2,
      2,
    },
    {
      7,
      1,
      6,
      1,
    },
  },
  nodes = {
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      name = 'math/add',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        {
          'IMPULSE',
          'In',
        },
        {
          3,
          'Operand 2',
        },
        {
          'ANY',
          'New Pin',
        },
      },
      name = 'script/scriptinput',
    },
    {
      loadArguments = {
        {
          'IMPULSE',
          'Out',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      name = 'print',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
  },
}