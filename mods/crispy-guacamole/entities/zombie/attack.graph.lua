script {
  nodes = {
    {
      name = 'script/scriptinput',
      initArguments = {
        {
          4,
          'Number',
        },
        {
          5,
          'String',
        },
      },
    },
    {
      name = 'init',
    },
    {
      name = 'test',
    },
    {
      name = 'string',
      initArguments = {
        'CRISPY',
      },
    },
    {
      name = 'math/multiply',
    },
    {
      name = 'number',
      initArguments = {
        0,
      },
    },
    {
      name = 'test',
    },
    {
      name = 'bool',
    },
    {
      name = 'test',
    },
    {
      name = 'bool',
    },
    {
      name = 'number',
      initArguments = {
        0,
      },
    },
    {
      name = 'string',
      initArguments = {
        'GUACAMOLE',
      },
    },
  },
  links = {
    {
      1,
      1,
      3,
      2,
    },
    {
      1,
      2,
      3,
      3,
    },
    {
      2,
      1,
      3,
      4,
    },
    {
      3,
      1,
      9,
      1,
    },
    {
      3,
      2,
      5,
      1,
    },
    {
      3,
      3,
      7,
      3,
    },
    {
      3,
      4,
      7,
      4,
    },
    {
      4,
      1,
      9,
      3,
    },
    {
      5,
      1,
      7,
      2,
    },
    {
      6,
      1,
      5,
      2,
    },
    {
      9,
      1,
      7,
      1,
    },
    {
      10,
      1,
      3,
      1,
    },
  },
}