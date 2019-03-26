return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      3,
      1,
    },
    {
      1,
      3,
      3,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              3,
              1,
              2,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                1,
                1,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                '',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
  },
}