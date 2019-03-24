return {
  links = {
    {
      2,
      1,
      5,
      1,
    },
    {
      3,
      1,
      2,
      1,
    },
    {
      3,
      2,
      8,
      1,
    },
    {
      3,
      2,
      6,
      1,
    },
    {
      3,
      3,
      2,
      2,
    },
    {
      3,
      4,
      12,
      1,
    },
    {
      6,
      1,
      12,
      2,
    },
    {
      6,
      1,
      14,
      1,
    },
    {
      6,
      1,
      15,
      1,
    },
    {
      7,
      1,
      10,
      3,
    },
    {
      8,
      1,
      9,
      1,
    },
    {
      9,
      1,
      13,
      1,
    },
    {
      9,
      2,
      16,
      1,
    },
    {
      9,
      3,
      7,
      1,
    },
    {
      10,
      1,
      2,
      3,
    },
    {
      11,
      1,
      7,
      2,
    },
    {
      12,
      1,
      2,
      4,
    },
    {
      13,
      1,
      10,
      1,
    },
    {
      14,
      1,
      16,
      2,
    },
    {
      15,
      1,
      13,
      2,
    },
    {
      16,
      1,
      10,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      loadArguments = {
        {
          'IMPULSE',
          'In',
        },
        {
          3397857525273543980,
          'Entity',
        },
        {
          4,
          'Entity Name',
        },
        {
          3,
          'Operand 1',
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
        false,
      },
      name = 'boolean',
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
      name = 'entity/getheading',
    },
    {
      name = 'math/add',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'math/vector3/vector3split',
    },
    {
      name = 'vector3',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      name = 'math/add',
    },
    {
      name = 'math/add',
    },
    {
      loadArguments = {
        'sin(heading)*0.5',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        'cos(heading)*0.5',
      },
      name = 'math/formula',
    },
    {
      name = 'math/add',
    },
  },
}